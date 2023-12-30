#include <common/Common.hpp>
#include <dwc/dwc_base64.h>
#include <dwc/dwc_login.h>
#include <platform/stdio.h>
#include <revolution/base/PPCArch.h>
#include <revolution/es.h>
#include <revolution/ios.h>
#include <revolutionex/nhttp/NHTTP.h>
#include <midnight/DolphinDevice.hpp>

////////////////////////////
// Wiimmfi Authentication //
////////////////////////////

// Pretend we're on version 96 of the Wiimmfi payload
#define WIIMMFI_PAYLOAD_VERSION "96"

// Pretend we're using the GCT version of the code
// Send the pre-encoded version of "LE-CODE GCT v1" (plus asterisks)
// TODO change this to a different value when we make the grand reveal
#define WIIMMFI_PATCHER_TYPE "TEUtQ09ERSBHQ1QgdjEgACoqKioqKioqKioqKioqKioqKioqKioqKioq"

// Don't send the IOS being used, Wiimmfi doesn't need this information
// Send the pre-encoded Dolphin default for the PAL game
#define WIIMMFI_IOS_VERSION "MzY*"

// Don't send the console type and region, Wiimmfi doesn't need this information
// Pretend we're a regular Wii on the PAL version
#define WIIMMFI_CONSOLE_TYPE "ffff-P"

// Default Dolphin chip ID, used in the code
#define DEFAULT_ECID_U 0x0D96E200
#define DEFAULT_ECID_M 0x1840C00D
#define DEFAULT_ECID_L 0x82BB08E8

// Store the base64-encoded console certificate so we don't need to request it again
bool sAuthdataParsed = false;
char sConsoleCert[513];

// Insert custom authentication parameters
kmCallDefCpp(0x800EDEE8, void, NHTTPReq* req, const char* key, const char* val) {

    // Original call
    NHTTPAddPostDataAscii(req, key, val);

    // Send payload version
    NHTTPAddPostDataAscii(req, "_payload_ver", WIIMMFI_PAYLOAD_VERSION);

    // Get the console's certificate (the real authentication method)
    // Only do this operation once
    if (!sAuthdataParsed) {
        ALIGN(32) char certBuf[0x180]; // IOS requires the output and the vector to be aligned by 32
        ALIGN(32) IOSIoVector vec = {certBuf, sizeof(certBuf)};
        s32 result = IOS_Ioctlv(__esFd, 0x1E, 0, 1, &vec);

        // If IOS call fails, bail
        if (result != IPC_OK)
            return;

        // Encode it
        DWC_Base64Encode(certBuf, sizeof(certBuf), sConsoleCert, sizeof(sConsoleCert)-1);
        sConsoleCert[sizeof(sConsoleCert)-1] = '\0';

        // Mark data as obtained successfully
        sAuthdataParsed = true;
    }

    // Send the certificate
    NHTTPAddPostDataAscii(req, "_val1", sConsoleCert);

    // Send patcher string
    NHTTPAddPostDataAscii(req, "_patcher", WIIMMFI_PATCHER_TYPE);

    // Send the IOS Version (but not the actual one)
    NHTTPAddPostDataAscii(req, "_game_ios", WIIMMFI_IOS_VERSION);

    // Send the console type (but not the actual one)
    // Wiimmfi here does a couple of things:
    // - Read 2 bytes at 0xCD8005A0
    // * On Wii, this is a mirror of 0xCD8001A0, a random clock register (PLLSYS) whose value will be 0xFFFF
    // * On other platforms, this is a register named LT_CHIPREVID, and the value will be 0xCAFE
    // - Try to open /title/00000001/00000002/data/macaddr.bin
    // * If the file exists, the console is a Wii Mini, so set the console type to 0x0C01
    // - Get the count of titles with id 0000000100000200
    // * The title is BC-NAND, which is vWii only, so if present set the console type to 0xCAFE
    // We will pretend to be a regular Wii console on the PAL game
    NHTTPAddPostDataAscii(req, "_console", WIIMMFI_CONSOLE_TYPE);

    // Send the device ID XOR'd with Dolphin's default value
    char deviceIdBuffer[28];
    sprintf(deviceIdBuffer, "%08x-%08x-%08x", PPCGetECID_U() ^ DEFAULT_ECID_U,
                                              PPCGetECID_M() ^ DEFAULT_ECID_M,
                                              PPCGetECID_L() ^ DEFAULT_ECID_L);
    NHTTPAddPostDataAscii(req, "_deviceID", deviceIdBuffer);

    // Check if the user is on Dolphin by opening its device, and if so send the Dolphin version
    if (DolphinDevice::Open()) {
        const char* version = DolphinDevice::GetVersion();
        if (version)
            NHTTPAddPostDataAscii(req, "_dolphin_ver", version);
    }

    // TODO: UPNP Port/Settings, Reconnect data
}

/////////////////////
// Error 60000 Fix //
/////////////////////

kmBranchDefCpp(0x800D05A8, 0x800D0610, void) {

    // Code ported from Wiimmfi without changes, i don't know exactly what it is doing...
    u64 userId = DWCi_Auth_GetConsoleUserId();
    stpLoginCnt->state = DWC_LOGIN_STATE_GPGETINFO;
    stpLoginCnt->tempLoginId.userId[0] = (userId >> 32) | 0x800; // this OR might be to prevent Nice FCs, not sure tho
    stpLoginCnt->tempLoginId.userId[1] = userId & 0xFFFFFFFF;
    DWCUserData* userData = stpLoginCnt->userdata;
    stpLoginCnt->tempLoginId.playerId = userData->pseudo.playerId;
    userData->flag = 1;
}
