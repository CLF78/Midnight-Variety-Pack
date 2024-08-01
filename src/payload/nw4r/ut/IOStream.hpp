#pragma once
#include "RuntimeTypeInfo.hpp"

namespace nw4r {
namespace ut {

class IOStream {
public:
    typedef void (*IOStreamCallback)(s32 result, IOStream* stream, void* arg);

    virtual const detail::RuntimeTypeInfo* GetRuntimeTypeInfo() const = 0;
    virtual ~IOStream() {}
    virtual void Close() = 0;
    virtual s32 Read(void* buf, u32 length);
    virtual bool ReadAsync(void* buf, u32 length, IOStreamCallback callback, void* arg) { return false; }
    virtual s32 Write(const void* buf, u32 length);
    virtual bool WriteAsync(const void* buf, u32 length, IOStreamCallback callback, void* arg);
    virtual bool IsBusy() const;
    virtual bool CanAsync() const = 0;
    virtual bool CanRead() const = 0;
    virtual bool CanWrite() const = 0;
    virtual u32 GetOffsetAlign() const = 0;
    virtual u32 GetSizeAlign() const = 0;
    virtual u32 GetBufferAlign() const = 0;

    bool available;
    s32 asyncResult;
    IOStreamCallback callback;
    void* arg;
};
size_assert(IOStream, 0x14);

} // namespace ut
} // namespace nw4r
