#include <kamek.h>
#include <nw4r/snd/SoundArchive.h>
#include <nw4r/snd/Util.h>
#include <nw4r/ut/BinaryFileHeader.h>

namespace nw4r {
namespace snd {
namespace detail {

struct SoundArchiveFile {

    struct Header {
        ut::BinaryFileHeader fileHeader;
        u32 symbolDataOffset;
        u32 symbolDataSize;
        u32 infoOffset;
        u32 infoSize;
        u32 fileImageOffset;
        u32 fileImageSize;
    };
    size_assert(Header, 0x28);

    struct Info {
        detail::Util::DataRef soundTableRef;
        detail::Util::DataRef bankTableRef;
        detail::Util::DataRef playerTableRef;
        detail::Util::DataRef fileTableRef;
        detail::Util::DataRef groupTableRef;
        detail::Util::DataRef soundArchivePlayerInfoRef;
    };
    size_assert(Info, 0x30);

    struct StringTreeNode {
        u16 flags;
        u16 bit;
        u32 leftIdx;
        u32 rightIdx;
        u32 strIdx;
        u32 id;
    };
    size_assert(StringTreeNode, 0x14);

    struct StringTree {
        u32 rootIdx;
        detail::Util::Table<StringTreeNode> nodeTable;
    };

    ulong detail_GetFileCount() const;
};

class SoundArchiveFileReader {
    public:
        ulong GetSoundCount() const; // custom function

        SoundArchive::SoundType GetSoundType(ulong soundId) const;
        bool ReadSoundInfo(ulong soundId, SoundArchive::SoundInfo* soundInfo) const;
        bool ReadStrmSoundInfo(ulong soundId, SoundArchive::StrmSoundInfo* info ) const;

        SoundArchiveFile::Header header;
        const SoundArchiveFile::Info* info;
        const void* stringBase;
        const detail::Util::Table<u32>* stringOffsetTable;
        const SoundArchiveFile::StringTree* stringTreeSound;
        const SoundArchiveFile::StringTree* stringTreePlayer;
        const SoundArchiveFile::StringTree* stringTreeGroup;
        const SoundArchiveFile::StringTree* stringTreeBank;
};
size_assert(SoundArchiveFileReader, 0x44);

} // namespace detail
} // namespace snd
} // namespace nw4r
