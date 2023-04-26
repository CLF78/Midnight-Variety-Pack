#include <kamek.h>
#include <nw4r/snd/SoundArchiveFile.h>
#include <nw4r/ut/DvdLockedFileStream.h>
#include <rvl/dvd/dvd.h>

namespace nw4r {
namespace snd {

class DvdSoundArchive : public SoundArchive {
public:
    class DvdFileStream;

    detail::SoundArchiveFileReader reader;
    DVDFileInfo fileInfo;
    bool open;
};
size_assert(DvdSoundArchive, 0x18C);

class DvdSoundArchive::DvdFileStream : public ut::DvdLockedFileStream {
public:
    DvdFileStream(const DVDFileInfo* fileInfo, u32 offset, s32 size); // Reimplemented
    DvdFileStream(s32 entryNum, u32 offset, s32 size); // Reimplemented

    virtual s32 Read(void* buf, u32 length);
    virtual void Seek(s32 offset, u32 origin);
    virtual u32 Tell() const;
    virtual u32 GetSize() const;

    s32 offset;
    u32 size;
};
size_assert(DvdSoundArchive::DvdFileStream, 0x78);


} // namespace snd
} // namespace nw4r
