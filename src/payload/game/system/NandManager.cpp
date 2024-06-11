#include <common/Common.hpp>

///////////////////////////////
// Savegame Expansion System //
///////////////////////////////

// NandManager::createStaticInstance() patch
// Initialize check error to NONE
kmWrite16(0x8052BE54, 0x93FE);
