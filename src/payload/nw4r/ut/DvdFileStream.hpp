#pragma once
#include "FileStream.hpp"
#include <revolution/dvd/dvd.h>
#pragma pack(push, 1)

namespace nw4r {
namespace ut {

class DvdFileStream : public FileStream {
public:

    struct DvdFileStreamInfo  {
        DVDFileInfo dvdInfo;
        DvdFileStream* stream;
    };

    virtual const detail::RuntimeTypeInfo* GetRuntimeTypeInfo() const;
    virtual ~DvdFileStream();

    virtual void Close();

    virtual s32 Read(void* buf, u32 length);

    virtual bool ReadAsync(void* buffer,
                           u32 length,
                           IOStreamCallback callback,
                           void* arg);

    virtual s32 Peek(void* buf, u32 length);

    virtual bool PeekAsync(void* buffer,
                           u32 length,
                           IOStreamCallback callback,
                           void* arg);

    virtual bool IsBusy() const;

    virtual void Seek(s32 offset, u32 origin);
    virtual u32 Tell() const;
    virtual u32 GetSize() const;
    virtual void Cancel();
    virtual bool CancelAsync(IOStreamCallback callback, void* arg);

    virtual bool CanAsync() const;
    virtual bool CanSeek() const;
    virtual bool CanRead() const;
    virtual bool CanWrite() const;
    virtual bool CanCancel() const;

    virtual u32 GetOffsetAlign() const;
    virtual u32 GetSizeAlign() const;
    virtual u32 GetBufferAlign() const;

    FilePosition filePosition;

    IOStreamCallback cancelCallback;
    void* cancelArg;
    volatile bool isCanceling;
    PAD(3);

    DvdFileStreamInfo fileInfo;
    s32 priority;
    volatile bool isBusy;
    bool closeOnDestroyFlag;
    bool closeEnableFlag;
};
size_assert(DvdFileStream, 0x6F);
#pragma pack(pop)

} // namespace ut
} // namespace nw4r
