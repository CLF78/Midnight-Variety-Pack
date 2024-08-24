#include "DvdSoundArchive.hpp"

namespace nw4r {
namespace snd {

DvdSoundArchive::DvdFileStream::DvdFileStream(s32 entryNum, u32 offset, s32 size) :
    DvdLockedFileStream(entryNum), offset((int)offset), size(size) {

    DvdLockedFileStream::Seek(this->offset, ut::FILE_STREAM_SEEK_BEGIN);
}

DvdSoundArchive::DvdFileStream::DvdFileStream(const DVDFileInfo* fileInfo, u32 offset, s32 size) :
    DvdLockedFileStream(fileInfo, false), offset((int)offset), size(size) {

    DvdLockedFileStream::Seek(this->offset, ut::FILE_STREAM_SEEK_BEGIN);
}

} // namespace snd
} // namespace nw4r
