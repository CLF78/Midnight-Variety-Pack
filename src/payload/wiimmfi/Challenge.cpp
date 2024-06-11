#include <common/Common.hpp>
#include <platform/string.h>
#include <wiimmfi/Challenge.hpp>

namespace Wiimmfi {
namespace Challenge {

char sChallenge[9];

void Save(const char* challenge) {
    LOG_DEBUG("Received challenge: %s", challenge);
    strcpy(sChallenge, challenge);
}

void Send(qr2_buffer* buffer) {

    // Check if the buffer has the required space for the extra key-value pair
    const u32 requiredSpace = sizeof("challenge") + sizeof(sChallenge);
    if (buffer->dataSize + requiredSpace > sizeof(buffer->data)) {
        LOG_ERROR("Not enough memory to append challenge!");
        return;
    }

    // Copy the key
    char* dest = &buffer->data[buffer->dataSize - 1];
    strcpy(dest, "challenge");

    // Copy the challenge
    strshift(dest, "challenge");
    strcpy(dest, sChallenge);

    // Update the buffer size and store the terminating null byte
    buffer->dataSize += requiredSpace;
    buffer->data[buffer->dataSize - 1] = '\0';
    LOG_DEBUG("Challenge appended to request successfully!");
}

} // namespace Challenge
} // namespace Wiimmfi
