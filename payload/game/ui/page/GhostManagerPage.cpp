#include <common/Common.hpp>

///////////////////////////////////////
// Patches for Custom Engine Classes //
///////////////////////////////////////

// Prevent overwriting CC when starting a ghost replay
kmWrite32(0x805E1EB0, 0x60000000);

// Prevent overwriting CC when starting a ghost race
kmWrite32(0x805E1D0C, 0x60000000);
