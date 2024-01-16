#include <common/Common.hpp>
#include <dwc/dwc_base64.h>
#include <platform/stdio.h>
#include <platform/stdlib.h>
#include <platform/string.h>
#include <revolution/base/PPCArch.h>
#include <revolution/es/es.h>
#include <revolution/os/OS.h>
#include <revolutionex/nhttp/NHTTP.h>
#include <midnight/DolphinDevice.hpp>
#include <wiimmfi/Auth.hpp>
#include <wiimmfi/Port.hpp>
#include <wiimmfi/Status.hpp>

namespace Wiimmfi {
namespace Auth {

char sConsoleCert[];

void AppendAuthParameters(NHTTPReq* req) {

    // Send payload version
    IGNORE_ERR(167)
    NHTTPAddPostDataAscii(req, "_payload_ver", PAYLOAD_VERSION);

    // Get the console's certificate (the real authentication method)
    // Only do this operation once
    static bool sCertObtained = false;

    if (!sCertObtained) {
        ALIGN(32) char certBuf[0x180]; // IOS requires the output to be aligned by 32
        s32 result = ES_GetDeviceCert((u8*)certBuf);

        // If IOS call fails, bail
        if (result != ES_ERR_OK)
            return;

        // Encode it
        int len = DWC_Base64Encode(certBuf, sizeof(certBuf), sConsoleCert, sizeof(sConsoleCert)-1);
        sConsoleCert[len] = '\0';

        // Mark data as obtained successfully
        sCertObtained = true;
    }

    // Send the certificate
    NHTTPAddPostDataAscii(req, "_val1", sConsoleCert);

    // Send patcher string
    NHTTPAddPostDataAscii(req, "_patcher", PATCHER_TYPE);

    // Send the IOS Version (but not the actual one)
    NHTTPAddPostDataAscii(req, "_game_ios", IOS_VERSION);

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
    NHTTPAddPostDataAscii(req, "_console", CONSOLE_TYPE);

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

    // TODO UPNP Port/Settings, Reconnect data
    UNIGNORE_ERR(167)
}

void ParseAuthResponse(const char* response) {

    // Response type: p2pport
    // Store the UPNP port to be used
    if (strstartw(response, RESPONSE_P2P)) {
        strshift(response, RESPONSE_P2P);
        Wiimmfi::Port::port = atoi(response);
    }

    // Response type: msg
    // Decode it and store it for display later
    else if (strstartw(response, RESPONSE_MSG)) {
        strshift(response, RESPONSE_MSG);

        // TODO finish this
        // 1) Get encoded len
        // 2) Compute decoded len
        // 3) Cap it to 790 and add 4
        // 4) Decode it with base64
        // 5) See rest of code for other steps
    }

    // Response type: xy
    // Decode the token and scramble it
    else if (strstartw(response, RESPONSE_XY)) {
        strshift(response, RESPONSE_XY);
        Status::DecodeToken(response);
    }
}

} // namespace Auth
} // namespace Wiimmfi
