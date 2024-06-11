#include <common/Common.hpp>

///////////////////////
// Custom Cup System //
///////////////////////

// SoundTriggerManager::ApplyTrigger() patch
// Disable Toad's Factory music reset trigger
// Credits: _tZ
kmWrite16(0x807198C4, 0x4800);
