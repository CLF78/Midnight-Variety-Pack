#include <kamek.h>
#include <nw4r/snd/BasicSound.h>

using namespace nw4r::snd::detail;

// Trim the SASR bit when setting the BasicSound id
extern "C" static void SetIDOverride(BasicSound* self, ulong soundId) {
    self->SetId(soundId & ~SASR_BIT);
}

// Glue code
kmCall(0x800A1BC4, SetIDOverride);
kmCall(0x800A1D6C, SetIDOverride);
kmCall(0x800A1F14, SetIDOverride);

// Trim the SASR bit when getting the ambient priority
kmCallDefCpp(0x800A1954, s32, BasicSound::AmbientInfo* info, ulong soundId) {
    return BasicSound::GetAmbientPriority(info, soundId & ~SASR_BIT);
}
