#include <common/Common.hpp>
#include <midnight/SoundExpansion.hpp>
#include <nw4r/snd/BasicSound.hpp>

namespace nw4r {
namespace snd {
namespace detail {

///////////////////////
// Custom SFX System //
///////////////////////

// Set the original sound ID
// Credits: stebler
REPLACE void BasicSound::SetId(ulong soundId) {
    REPLACED(soundId & ~SASR_BIT);
}

// Get the ambient priority from the original sound ID
// Credits: stebler
REPLACE_STATIC s32 BasicSound::GetAmbientPriority(AmbientInfo* info, ulong soundId) {
    return REPLACED(info, soundId & ~SASR_BIT);
}

} // namespace detail
} // namespace snd
} // namespace nw4r
