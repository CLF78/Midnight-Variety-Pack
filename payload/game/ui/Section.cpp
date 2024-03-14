#include <common/Common.hpp>

///////////////////////////////
// Savegame Expansion System //
///////////////////////////////

// Section::addPages() / Section::addActivePages() patches
// Replace CannotSavePage with MiiCorruptPage
kmWritePointer(0x808BC844, 0x8062C514);
kmWritePointer(0x808BCA80, 0x806301BC);
