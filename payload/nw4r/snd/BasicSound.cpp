#include <common/Common.hpp>
#include <midnight/SoundExpansion.hpp>
#include <nw4r/snd/BasicSound.hpp>

namespace nw4r {
namespace snd {
namespace detail {

///////////////////////
// Custom SFX System //
///////////////////////

// nw4r::snd::BasicSound::SetId() override
// Trim the SASR bit before calling the function
// Credits: stebler
kmHookFn void SetIDOverride(BasicSound* self, ulong soundId) {
    self->SetId(soundId & ~SASR_BIT);
}

// Glue code
kmCall(0x800A1BC4, SetIDOverride);
kmCall(0x800A1D6C, SetIDOverride);
kmCall(0x800A1F14, SetIDOverride);

// nw4r::snd::BasicSound::GetAmbientPriority() override
// Trim the SASR bit before calling the function
// Credits: stebler
kmCallDefCpp(0x800A1954, s32, BasicSound::AmbientInfo* info, ulong soundId) {
    return BasicSound::GetAmbientPriority(info, soundId & ~SASR_BIT);
}

} // namespace detail
} // namespace snd
} // namespace nw4r
