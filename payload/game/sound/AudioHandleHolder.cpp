#include <common/Common.hpp>

///////////////////////
// Custom Cup System //
///////////////////////

// AudioHandleHolder::PrepareSpecial() override
// Remove Toad's Factory special delayed music start
kmBranch(0x806F8ED8, 0x806F8DCC);
