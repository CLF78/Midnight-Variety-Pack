#include <kamek.h>
#include <nw4r/snd/SoundArchive.h>

ulong nw4r::snd::SoundArchive::GetSoundCount() const {
    return fileReader->GetSoundCount();
}
