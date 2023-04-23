#include <kamek.h>
#include <nw4r/ut/Lock.h>
#include <rvl/os/OS.h>
#include <stdlib/stdio.h>
#include <stdlib/string.h>
#include "sfxreplace/DVDFileStream.h"

namespace nw4r {
namespace snd {

DVDFileStream::DVDFileStream(const char* path) {

    // Ensure the path fits the buffer, else clear it
    if (snprintf(this->path, sizeof(this->path), "%s", path) >= sizeof(this->path))
        memset(this->path, 0, sizeof(this->path));
}

DVDFileStream::DVDFileStream(DVDFileInfo* file, u32 start, u32 size) {

    // Copy the data over
    this->file = *file;
    this->start = start;
    this->size = size;
}

DVDFileStream::~DVDFileStream() {}

void DVDFileStream::Close() {

    // Clear the path, the file and the cancelled flag
    memset(this->path, 0, sizeof(this->path));
    memset(&this->file, 0, sizeof(this->file));
    cancelled = false;
}

s32 DVDFileStream::Read(void* dst, u32 size) {

    // Lock out other threads
    {
        ut::AutoInterruptLock lock;
        while (!OSTryLockMutex(&mutex)) {
            OSSleepThread(&queue);
            if (cancelled)
                return -1;
        }
    }

    // If the file isn't open, bail
    if (!Open())
        return -1;

    // Try to read the file
    bool result = DVDReadPrio(&file, dst, size, start + offset, 2);

    // Unlock other threads
    {
        ut::AutoInterruptLock lock;
        OSUnlockMutex(&mutex);
        OSWakeupThread(&queue);
    }

    // If the read failed, bail
    if (!result)
        return -1;

    // Update offset and return size
    offset += size;
    return size;
}

bool DVDFileStream::CanAsync() {
    return false;
}

bool DVDFileStream::CanRead() {
    return true;
}

bool DVDFileStream::CanWrite() {
    return false;
}

u32 DVDFileStream::GetOffsetAlign() {
    return 0x20;
}

u32 DVDFileStream::GetSizeAlign() {
    return 0x20;
}

u32 DVDFileStream::GetBufferAlign() {
    return 0x20;
}

u32 DVDFileStream::GetSize() {

    // Ensure the file is open
    if (!Open())
        return 0;

    // Ensure the size is valid
    return min(OSRoundUp32B(file.length), size);
}

void DVDFileStream::Seek(s32 offset, ut::SeekOrigin origin) {
    switch (origin) {
        case ut::FILE_STREAM_SEEK_BEGIN:
            this->offset = offset;
            break;
        case ut::FILE_STREAM_SEEK_CURRENT:
            this->offset += offset;
            break;
        case ut::FILE_STREAM_SEEK_END:
            this->offset = GetSize() - offset;
            break;
    }
}

void DVDFileStream::Cancel() {
    ut::AutoInterruptLock lock;

    cancelled = true;
    OSWakeupThread(&queue);
}

bool DVDFileStream::CanSeek() {
    return true;
}

bool DVDFileStream::CanCancel() {
    return true;
}

u32 DVDFileStream::Tell() {
    return offset;
}

bool DVDFileStream::Open() {
    if (!path)
        return false;

    // TODO actual logic
    return true;
}

OSMutex DVDFileStream::mutex;
OSThreadQueue DVDFileStream::queue;

} // namespace snd 
} // namespace nw4r
