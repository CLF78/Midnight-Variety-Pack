#pragma once
#include <gs/qr2/qr2.h>

namespace Wiimmfi {
namespace Challenge {

// The challenge size (including the null character)
static const int CHALLENGE_SIZE = 9;

// The key used to send the challenge
static const char CHALLENGE_KEY[] = "challenge";

// Saves the received challenge
void Save(const char* challenge);

// Sends the saved challenge
void Send(qr2_buffer* buffer);

} // namespace Challenge
} // namespace Wiimmfi
