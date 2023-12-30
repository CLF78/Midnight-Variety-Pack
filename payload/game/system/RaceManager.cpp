#include <common/Common.hpp>

//////////////////////////////
// Patches for Lap Modifier //
//////////////////////////////

// Allow custom lap count outside of competitions
kmWrite32(0x805328B0, 0x60000000);
kmWrite32(0x805336B8, 0x60000000);
kmWrite32(0x80534350, 0x60000000);
kmWrite32(0x80534BBC, 0x60000000);
