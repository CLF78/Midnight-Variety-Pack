#pragma once
#include "DvdFileStream.hpp"
#include <revolution/os/OSMutex.h>

namespace nw4r {
namespace ut {

class DvdLockedFileStream : public DvdFileStream {
public:
    virtual const detail::RuntimeTypeInfo* GetRuntimeTypeInfo() const;

    DvdLockedFileStream();
    explicit DvdLockedFileStream(const char* path);
    explicit DvdLockedFileStream(s32 entryNum);
    explicit DvdLockedFileStream(const DVDFileInfo* openedFile, bool closeEnable = true);
    virtual ~DvdLockedFileStream();

    virtual void Close();
    virtual s32 Read(void* buf, u32 length);
    virtual bool ReadAsync(void* buf, u32 length, IOStreamCallback callback, void* arg);
    virtual s32 Peek(void* buf, u32 length);
    virtual bool PeekAsync(void* buf, u32 length, IOStreamCallback callback, void* arg);
    virtual void Cancel();
    virtual bool CanAsync() const;

    volatile bool cancelFlag;
};
size_assert(DvdLockedFileStream, 0x70);

} // namespace ut
} // namespace nw4r
