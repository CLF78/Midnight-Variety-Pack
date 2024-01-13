#include <common/Common.hpp>

////////////////////
// Game Bug Fixes //
////////////////////

// Prevent invalid profile identifiers from crashing the game
// Original Gecko code by MrBean35000vr
kmBranch(0x805D2EF8, 0x805D2EBC);
kmBranch(0x805D2F00, 0x805D2EBC);
