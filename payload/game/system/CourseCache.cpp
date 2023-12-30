#include <common/Common.hpp>

///////////////////////////////////
// Patches for Custom Cup System //
///////////////////////////////////

// Disable the course cache altogether
kmBranch(0x8053FD70, 0x8053FDA4); // skip constructor
kmWrite32(0x80542D9C, 0x60000000); // skip initialization
