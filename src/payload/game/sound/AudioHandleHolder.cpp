#include "AudioHandleHolder.hpp"

///////////////////////
// Custom Cup System //
///////////////////////

// Remove Toad's Factory special delayed music start
REPLACE void AudioHandleHolder::prepareSpecial(u32 soundId, bool unk) {
    prepare(soundId, unk);
}
