#include <kamek.h>
#include <dwc/dwc_base64.h>
#include <rvl/base/PPCArch.h>
#include <rvl/es/es.h>
#include <rvl/ios/IOS.h>
#include <rvl/nhttp/NHTTP.h>
#include <stdlib/stdio.h>

// Wiimmfi authentication code, ported from payload v93 by CLF78
#define PAYLOAD_VERSION "93"

// Pretend we're using the GCT version of the code
// This is the encoded version of "LE-CODE GCT v1  **************************"
// TODO change this to a different value when we make the grand reveal
#define b64PatcherType "TEUtQ09ERSBHQ1QgdjEgACoqKioqKioqKioqKioqKioqKioqKioqKioq"

// Don't send the actual IOS being used, Wiimmfi doesn't need this information
// This is the already encoded Dolphin default for the PAL game
#define b64IosVer "MzY*"

// Don't send the actual console type and region, Wiimmfi doesn't need this information
// Disguise as a regular Wii on the PAL version
#define consoleType "ffff-P"

// Default Dolphin chip ID
#define DEFAULT_ECID_U 0x0D96E200
#define DEFAULT_ECID_M 0x1840C00D
#define DEFAULT_ECID_L 0x82BB08E8

// Define this bool so IOS calls aren't repeated between logins
bool authdata_parsed = false;

// Base64 buffer for authentication
char b64ConsoleCert[513];

// Hook
kmCallDefCpp(0x800EDEE8, void, NHTTPReq* req, const char* key, const char* val) {

    // Original call
    NHTTPAddPostDataAscii(req, key, val);

    // Send payload version
    NHTTPAddPostDataAscii(req, "_payload_ver", PAYLOAD_VERSION);

    // Get the console's certificate (the real authentication method)
    // Only do this operation once
    if (!authdata_parsed) {
        char certBuf[0x180] __attribute__ ((aligned(32))); // IOS requires the output and the vector to be aligned by 32
        IOSIoVector vec __attribute__ ((aligned(32))) = {&certBuf, sizeof(certBuf)};
        s32 result = IOS_Ioctlv(__esFd, 0x1E, 0, 1, &vec);

        // If IOS call fails, bail
        if (result != 0)
            return;

        // Encode it
        DWC_Base64Encode((const char*)&certBuf, sizeof(certBuf), (char*)&b64ConsoleCert, sizeof(b64ConsoleCert)-1);
        b64ConsoleCert[sizeof(b64ConsoleCert)-1] = '\0';
    }

    // Send the certificate
    NHTTPAddPostDataAscii(req, "_val1", (const char*)&b64ConsoleCert);

    // Send patcher string
    NHTTPAddPostDataAscii(req, "_patcher", b64PatcherType);

    // Send the IOS Version (but not the actual one)
    NHTTPAddPostDataAscii(req, "_game_ios", b64IosVer);

    // Send the console type (but not the actual one)
    // Wiimmfi here does a couple of things:
    // - Read 2 bytes at 0xCD8005A0
    // * On Wii, this is a mirror of 0xCD8001A0, a random clock register (PLLSYS) whose value will be 0xFFFF
    // * On vWii, this is a register named LT_CHIPREVID, and the value will be 0xCAFE
    // - Try to open /title/00000001/00000002/data/macaddr.bin
    // * If the file exists, the console is a Wii Mini, so set console type to 0x0C01
    // - Get the count of titles with id 0000000100000200
    // * The title is BC-NAND, which is vWii only
    // We will pretend to be a regular Wii console on the PAL game
    NHTTPAddPostDataAscii(req, "_console", consoleType);

    // Send the device ID XOR'd with Dolphin's default value
    u32 ECID_U = PPCmfecid_u();
    u32 ECID_M = PPCMfecid_m();
    u32 ECID_L = PPCMfecid_l();
    char deviceIdBuffer[28];
    snprintf(deviceIdBuffer, sizeof(deviceIdBuffer), "%08x-%08x-%08x", ECID_U ^ DEFAULT_ECID_U,
                                                                       ECID_M ^ DEFAULT_ECID_M,
                                                                       ECID_L ^ DEFAULT_ECID_L);
    NHTTPAddPostDataAscii(req, "_deviceID", deviceIdBuffer);

    // TODO Check if the user is on Dolphin by opening the /dev/dolphin interface and sending the Dolphin version
    // TODO: UPNP Port/Settings, Reconnect data

    // Mark data as obtained successfully
    authdata_parsed = true;
}
