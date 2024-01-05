#include <common/Common.hpp>
#include <dwc/dwc_base64.h>
#include <dwc/dwc_match.h>
#include <game/system/RKSystem.hpp>
#include <gs/gp/gpi.h>
#include <nw4r/ut/Lock.hpp>
#include <platform/new.hpp>
#include <platform/stdio.h>
#include <platform/string.h>
#include <wiimmfi/Status.hpp>

namespace Wiimmfi {
namespace Status {

char* token;

char scrambledToken[96];
const int OFFSET = 0x20;

void DecodeToken(const char* encodedToken) {

    // Get the decoded token size
    IGNORE_ERR(384)
    int encodedLen = strlen(encodedToken);
    int decodedLen = DWC_Base64Decode(encodedToken, encodedLen, nullptr, 0);
    token = new (KAMEK_HEAP, 4) char[decodedLen+1];
    UNIGNORE_ERR(384)

    // Decode the token
    decodedLen = DWC_Base64Decode(encodedToken, encodedLen, token, decodedLen);
    token[decodedLen] = '\0';

    // Scramble the token
    // Start by filling the array with garbage data
    for (int i = 0; i < sizeof(scrambledToken); i++) {
        scrambledToken[i] = i + OFFSET;
    }

    // Check if the token was decoded correctly
    if (decodedLen > 0) {

        // Run a substitution cipher on the token
        const char key[] = "0123456789,abcdefghijklmnopqrstuvwxyz|=+-_";
        for (int i = 0; i < decodedLen + 1 && i < sizeof(key); i++) {
            char c = token[i];
            char pos = key[i];
            scrambledToken[pos - OFFSET] = c;
        }
    }
}

void ScrambleMessage(char* msg, int msgLen) {

    // Check that the token was obtained, if not bail
    if (!token)
        return;

    // Scramble the message otherwise
    for (int i = 0; i < msgLen; i++) {
        u8 c = msg[i] - OFFSET;
        if (c < sizeof(scrambledToken)-1)
            msg[i] = scrambledToken[c];
    }
}

void SendMessage(const char* key, const char* value, int integerValue) {

    // Disable interrupts
    nw4r::ut::AutoInterruptLock lock;

    // Check that the match control structure exists
    if (!stpMatchCnt || !stpMatchCnt->gpConnection)
        return;

    // Get connection
    GPConnection conn = stpMatchCnt->gpConnection;

    // Here Wiimmfi does some weird check with the connection pointer:
    // if ((u8*)conn + 0x70000210 > 0x50000000) then quit
    // Given that it always evaluates to false, i will leave it out unless issues arise

    // Print the message to the buffer
    char buffer[599];
    int len = snprintf(buffer, sizeof(buffer), "\\xy\\%s\\v\\1\\id\\%d\\msg\\%s\\final\\",
                       key, integerValue, value);

    // If the printed string did not fit in the buffer, bail
    if (len > sizeof(buffer))
        return;

    // Another weird check here:
    // if (buffer + 4 > buffer + len - 7) quit (doing (len < 3) was obviously too difficult)
    // Either way this will never be true because the format string is longer, so i'm leaving it out

    // Scramble the message and send it
    ScrambleMessage(buffer + sizeof("\\xy\\")-1, len - sizeof("\\final\\"));
    gpiAppendStringToBuffer(conn, &(*conn)->outputBuffer, buffer);
}

} // namespace Status
} // namespace Wiimmfi
