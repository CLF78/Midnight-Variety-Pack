#include <kamek.h>
#include <nw4r/ut/IOStream.h>

namespace nw4r {
namespace ut {

enum SeekOrigin {
    FILE_STREAM_SEEK_BEGIN,
    FILE_STREAM_SEEK_CURRENT,
    FILE_STREAM_SEEK_END
};

class FileStream : public IOStream {
public:
    virtual const detail::RuntimeTypeInfo* GetRuntimeTypeInfo() const = 0;
    virtual u32 GetSize() = 0;
    virtual void Seek(s32 offset, u32 origin);
    virtual void Cancel();
    virtual bool CancelAsync(IOStreamCallback callback, void* arg);
    virtual bool CanSeek() const = 0;
    virtual bool CanCancel() const = 0;
    virtual u32 Tell() const = 0;

    class FilePosition {
    public:
        u32 fileSize;
        u32 position;
    };
};

} // namespace ut
} // namespace nw4r
