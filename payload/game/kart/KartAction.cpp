#include <common/Common.hpp>

////////////////////
// Game Bug Fixes //
////////////////////

// Do not replace bomb damage with spinouts
kmWrite16(0x80569F68, 0x4800);
