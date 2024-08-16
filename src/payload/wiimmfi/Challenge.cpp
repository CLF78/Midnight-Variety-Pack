#include "Challenge.hpp"
#include <platform/string.h>

namespace Wiimmfi {
namespace Challenge {

char sChallenge[CHALLENGE_SIZE];

void Save(const char* challenge) {
    LOG_DEBUG("Received challenge: %s", challenge);
    strlcpy(sChallenge, challenge, sizeof(sChallenge));
}

void Send(qr2_buffer* buffer) {

    // Check if the buffer has the required space for the extra key-value pair
    const u32 requiredSpace = sizeof(CHALLENGE_KEY) + sizeof(sChallenge);
    if (buffer->dataSize + requiredSpace > sizeof(buffer->data)) {
        LOG_ERROR("Not enough memory to append challenge!");
        return;
    }

    // Copy the key
    char* dest = &buffer->data[buffer->dataSize - 1];
    strlcpy(dest, CHALLENGE_KEY, sizeof(CHALLENGE_KEY));

    // Copy the challenge
    strshift(dest, CHALLENGE_KEY);
    strlcpy(dest, sChallenge, sizeof(sChallenge));

    // Update the buffer size and store the terminating null byte
    buffer->dataSize += requiredSpace;
    buffer->data[buffer->dataSize - 1] = '\0';
    LOG_DEBUG("Challenge appended to request successfully!");
}

} // namespace Challenge
} // namespace Wiimmfi
