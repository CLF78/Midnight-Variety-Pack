#include <kamek.h>
#include <nw4r/snd/SoundArchiveFile.h>
#include <nw4r/snd/Util.h>

ulong nw4r::snd::detail::SoundArchiveFileReader::GetSoundCount() const {
    u32 offset = info->soundTableRef.value;
    const Util::Table<u32>* table =  (Util::Table<u32>*)((u8*)(info) + offset);
    return table->count;
}
