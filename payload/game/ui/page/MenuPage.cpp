#include <common/Common.hpp>

///////////////////////////////////
// Patches for Custom Cup System //
///////////////////////////////////

// Prevent the game from rewriting the last selected course/stage
kmBranch(0x80837DB0, 0x80837E2C);
