#include "Auth.hpp"
#include "Port.hpp"
#include "Status.hpp"
#include <mvp/util/DolphinDevice.hpp>
#include <platform/stdio.h>
#include <platform/string.h>
#include <revolution/base/PPCArch.h>
#include <revolution/base/PPCReg.h>
#include <revolution/es/es.h>
#include <revolution/os/OSLoMem.h>
#include <revolution/os/OSTitle.h>

namespace Wiimmfi {
namespace Auth {

char sConsoleCert[DWC_Base64GetEncodedSize(IOSECCCertSize)+1];
char sConsoleAssignMessageBuffer[796];
wchar_t* sConsoleAssignMessage;

u32 GetConsoleType() {

    // Test for Dolphin by reading the LT_CHIPREVID register
    if (__LT_CHIPREVID == CONSOLE_DOLPHIN) {
        LOG_DEBUG("Detected console type: Dolphin");
        return CONSOLE_DOLPHIN;
    }

    // Test for Wii Mini by opening /title/00000001/00000002/data/macaddr.bin
    const int macAddrFd = IOS_Open("/title/00000001/00000002/data/macaddr.bin", IPC_OPEN_READ);
    if (macAddrFd >= 0) {
        IOS_Close(macAddrFd); // Close the file back
        LOG_DEBUG("Detected console type: Wii Mini");
        return CONSOLE_WII_MINI;
    }

    // Test for vWii by checking for the BC-NAND title
    if (OSIsTitleInstalled(0x0000000100000200)) {
        LOG_DEBUG("Detected console type: vWii");
        return CONSOLE_WII_U;
    }

    // None of the checks passed, we are a regular Wii
    LOG_DEBUG("Detected console type: Wii");
    return CONSOLE_WII;
}

void AppendAuthParameters(NHTTPReq* req) {

    // Send payload version
    LOG_DEBUG("Started Wiimmfi authentication procedure.");
    LOG_DEBUG("Sending payload version: %s", PAYLOAD_VERSION);
    NHTTPAddPostDataAscii(req, "_payload_ver", PAYLOAD_VERSION);

    // Get the console's certificate (the real authentication method)
    // Only do this operation once
    static bool sCertObtained = false;

    if (!sCertObtained) {
        ALIGN(32) char certBuf[IOSECCCertSize]; // IOS requires the output to be aligned by 32
        const s32 result = ES_GetDeviceCert((u8*)certBuf);

        // If IOS call fails, bail
        if (result != ES_ERR_OK) {
            LOG_ERROR("Failed to get device certificate.");
            return;
        }

        // Encode it
        const int len = DWC_Base64Encode(certBuf, sizeof(certBuf), sConsoleCert, sizeof(sConsoleCert));
        sConsoleCert[len] = '\0';

        // Mark data as obtained successfully
        sCertObtained = true;
    } else {
        LOG_DEBUG("Already obtained device certificate for authentication.");
    }

    // Send the certificate
    LOG_DEBUG("Sending device certificate: %s", sConsoleCert);
    NHTTPAddPostDataAscii(req, "_val1", sConsoleCert);

    // Send patcher string
    // Do not send the patcher version, as that is only used with WSZST-patched DOLs
    LOG_DEBUG("Sending patcher type: OpenPayload");
    NHTTPAddPostDataAscii(req, "_patcher", PATCHER_TYPE);

    // Get the console type and region
    // Only do this operation once
    static bool sConsoleTypeObtained = false;
    static char sConsoleTypeBuffer[7];
    if (!sConsoleTypeObtained) {
        sprintf(sConsoleTypeBuffer, "%04x-%c", GetConsoleType(), __OSBootInfo.diskInfo.gameName[3]);
        sConsoleTypeObtained = true;
    } else {
        LOG_DEBUG("Already obtained console type and region.");
    }

    // Send it over
    LOG_DEBUG("Sending console type and region: %s", sConsoleTypeBuffer);
    NHTTPAddPostDataAscii(req, "_console", sConsoleTypeBuffer);

    // Send the device ID XOR'd with Dolphin's default value
    char deviceIdBuffer[28];
    sprintf(deviceIdBuffer, "%08x-%08x-%08x", PPCGetECID_U() ^ DEFAULT_ECID_U,
                                              PPCGetECID_M() ^ DEFAULT_ECID_M,
                                              PPCGetECID_L() ^ DEFAULT_ECID_L);
    LOG_DEBUG("Sending device ID: %s", deviceIdBuffer);
    NHTTPAddPostDataAscii(req, "_deviceID", deviceIdBuffer);

    // Check if the user is on Dolphin by opening its device, and if so send the Dolphin version
    if (DolphinDevice::IsOpen()) {
        const char* version = DolphinDevice::GetVersion();
        if (version) {
            LOG_DEBUG("Sending Dolphin version: %s", version);
            NHTTPAddPostDataAscii(req, "_dolphin_ver", version);
        } else {
            LOG_ERROR("Failed to get Dolphin version.");
        }
    }

    // If the user has set an UPNP port, send it over
    if (Wiimmfi::Port::sUserPort) {

        // Convert to string
        char portBuffer[6];
        snprintf(portBuffer, sizeof(portBuffer), "%d", Wiimmfi::Port::sUserPort);
        LOG_DEBUG("Sending user port: %s", portBuffer);

        // Encode to Base64
        char b64UserPort[DWC_Base64GetEncodedSize(sizeof(portBuffer))+1];
        const int len = DWC_Base64Encode(portBuffer, sizeof(portBuffer), b64UserPort, sizeof(b64UserPort));
        b64UserPort[len] = '\0';

        // Send it
        NHTTPAddPostDataAscii(req, "_upnpport", b64UserPort);
    } else {
        LOG_DEBUG("No user port found. Skipping...");
    }

    LOG_DEBUG("Wiimmfi authentication completed.");
}

void ParseAuthResponse(const char* response) {

    // Response type: P2P port
    // Store the UPNP port to be used
    if (strstart(response, RESPONSE_P2PPORT)) {
        strshift(response, RESPONSE_P2PPORT);
        LOG_DEBUG("Received Wiimmfi P2P port: %s", response);
        Wiimmfi::Port::sPort = strtoul(response, nullptr, 10);
    }

    // Response type: console assignment message
    // Decode the message and store it for display later
    else if (strstart(response, RESPONSE_CONSOLE_ASSIGN)) {
        strshift(response, RESPONSE_CONSOLE_ASSIGN);
        LOG_DEBUG("Received Wiimmfi console assignment message");

        // Get encoded and decoded message length
        // If it doesn't fit the buffer, bail
        const size_t encodedLen = strlen(response);
        u32 decodedLen = DWC_Base64Decode(response, encodedLen, nullptr, 0);
        if (decodedLen > sizeof(sConsoleAssignMessageBuffer) - sizeof(wchar_t)) {
            LOG_ERROR("Message exceeds buffer size, discarding...");
            return;
        }

        // Decode the message and set the pointer to indicate a successful decode
        // The message is already in UTF-16 format, so we do not need to convert it
        decodedLen = DWC_Base64Decode(response, encodedLen, sConsoleAssignMessageBuffer,
                                      sizeof(sConsoleAssignMessageBuffer) - sizeof(wchar_t));
        sConsoleAssignMessageBuffer[decodedLen] = '\0';
        sConsoleAssignMessageBuffer[decodedLen+1] = '\0';
        sConsoleAssignMessage = (wchar_t*)sConsoleAssignMessageBuffer;
    }

    // Response type: token
    // Decode the token and scramble it
    else if (strstart(response, RESPONSE_TOKEN)) {
        strshift(response, RESPONSE_TOKEN);
        LOG_DEBUG("Received Wiimmfi scramble token.");
        Status::DecodeToken(response);
    }

    else {
        LOG_WARN("Unknown response: %s", response);
    }
}

} // namespace Auth
} // namespace Wiimmfi
