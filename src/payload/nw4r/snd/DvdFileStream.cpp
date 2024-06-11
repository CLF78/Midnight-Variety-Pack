#include <common/Common.hpp>
#include <nw4r/snd/DvdSoundArchive.hpp>

namespace nw4r {
namespace snd {

DvdSoundArchive::DvdFileStream::DvdFileStream(s32 entryNum, u32 offset, s32 size):
    ut::DvdLockedFileStream(entryNum),
    offset(offset),
    size(size) {

    ut::DvdLockedFileStream::Seek(this->offset, ut::FILE_STREAM_SEEK_BEGIN);
}

DvdSoundArchive::DvdFileStream::DvdFileStream(const DVDFileInfo* fileInfo, u32 offset, s32 size):
    ut::DvdLockedFileStream(fileInfo, false),
    offset(offset),
    size(size) {

    ut::DvdLockedFileStream::Seek(this->offset, ut::FILE_STREAM_SEEK_BEGIN);
}

} // namespace snd
} // namespace nw4r
