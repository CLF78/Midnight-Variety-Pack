#include <common/Common.hpp>
#include <dwc/dwc_base64.h>
#include <dwc/dwc_match.h>
#include <game/system/RKSystem.hpp>
#include <gs/gp/gpi.h>
#include <nw4r/ut/Lock.hpp>
#include <platform/stdio.h>
#include <platform/string.h>
#include <wiimmfi/Status.hpp>

namespace Wiimmfi {
namespace Status {

char* sToken;
char sScrambledToken[96];

void DecodeToken(const char* encodedToken) {

    // Get the decoded token size
    IGNORE_ERR(384)
    int encodedLen = strlen(encodedToken);
    int decodedLen = DWC_Base64Decode(encodedToken, encodedLen, nullptr, 0);
    sToken = new (KAMEK_HEAP, 32) char[decodedLen+1];
    UNIGNORE_ERR(384)

    // Decode the token
    decodedLen = DWC_Base64Decode(encodedToken, encodedLen, sToken, decodedLen);
    sToken[decodedLen] = '\0';

    // Scramble the token
    // Start by filling an ASCII table
    for (int i = 0; i < strlenc(sScrambledToken); i++) {
        sScrambledToken[i] = i + ' ';
    }

    // Check if the token was decoded correctly
    if (decodedLen > 0) {

        // Run ASCII substitution
        static const char key[] = "0123456789,abcdefghijklmnopqrstuvwxyz|=+-_";
        for (int i = 0; i < decodedLen && i < strlenc(key); i++) {
            char c = sToken[i];
            char pos = key[i];
            sScrambledToken[pos - ' '] = c;
        }
    }
}

void ScrambleMessage(char* msg, int msgLen) {
    for (int i = 0; i < msgLen; i++) {
        u8 c = msg[i] - ' ';
        if (c < strlenc(sScrambledToken))
            msg[i] = sScrambledToken[c];
    }
}

void SendMessage(const char* key, const char* value, int integerValue) {

    // Check that the match control structure exists
    if (!stpMatchCnt || !stpMatchCnt->gpConnection)
        return;

    // Log entry
    LOG_DEBUG("Sending report message %s=%s,%d", key, value, integerValue);

    // Get connection
    GPConnection conn = stpMatchCnt->gpConnection;
    if (!*conn) {
        LOG_WARN("Connection not initialized, discarding message...");
        return;
    }

    // Print the message to the buffer
    char buffer[599];
    int len = snprintf(buffer, sizeof(buffer), "\\xy\\%s\\v\\1\\id\\%d\\msg\\%s\\final\\",
                       key, integerValue, value);

    // If the printed string did not fit in the buffer, bail
    if (len > sizeof(buffer)) {
        LOG_WARN("Message length exceeded buffer (size: %d), discarding...", len);
        return;
    }

    // Scramble the message
    ScrambleMessage(buffer + strlenc(STATUS_HEADER),
                    len - strlenc(STATUS_TERMINATOR) - strlenc(STATUS_HEADER));

    // Lock interrupts and append message
    {
        nw4r::ut::AutoInterruptLock lock;
        gpiAppendStringToBuffer(conn, &(*conn)->outputBuffer, buffer);
    }
}

} // namespace Status
} // namespace Wiimmfi
