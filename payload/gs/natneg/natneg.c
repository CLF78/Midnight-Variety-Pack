#include <common/Common.h>

/////////////////
// Fast NATNEG //
/////////////////

// NegotiateThink() patch
// Reduce CONNECT_PING retry time from 700 to 80
// Credits: Wiimmfi
kmWrite16(0x8011B47A, 80);

// NegotiateThink() patch
// Do not wait the retry time in case of successful NATNEG
// Credits: Wiimmfi
kmWrite32(0x8011B4B0, 0x60000000);
