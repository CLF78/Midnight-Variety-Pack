#include <common/Common.hpp>

///////////////////////
// Custom Cup System //
///////////////////////

// BattleStageSelectPage::onActivate() patch
// Turn off background movies
kmWrite32(0x8083CD94, 0x60000000);
