#include <common/Common.hpp>
#include <nw4r/snd/SoundArchiveFile.hpp>
#include <nw4r/snd/Util.hpp>

namespace nw4r {
namespace snd {
namespace detail {

ulong nw4r::snd::detail::SoundArchiveFileReader::GetSoundCount() const {
    u32 offset = info->soundTableRef.value;
    const Util::Table<u32>* table =  (Util::Table<u32>*)((u8*)(info) + offset);
    return table->count;
}

///////////////////////
// Custom SFX System //
///////////////////////

// nw4r::snd::SoundArchiveFileReader::ReadSoundArchivePlayerInfo() patches
// Increase the amount of playable BRSTMs at once to 64
kmWrite32(0x8009F184, 0x38000040);
kmWrite32(0x8009F18C, 0x38000040);

} // namespace detail
} // namespace snd
} // namespace nw4r
