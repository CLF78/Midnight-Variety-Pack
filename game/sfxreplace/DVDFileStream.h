#include <kamek.h>
#include <nw4r/ut/FileStream.h>
#include <rvl/dvd/dvd.h>
#include <rvl/os/OSMutex.h>
#include <rvl/os/OSThread.h>

namespace nw4r {
namespace snd {

class DVDFileStream : public ut::FileStream {
public:
    DVDFileStream(const char* path);
    DVDFileStream(DVDFileInfo* file, u32 start, u32 size);

    virtual const ut::detail::RuntimeTypeInfo* GetRuntimeTypeInfo() const { return nullptr; }
    virtual ~DVDFileStream();
    virtual void Close();
    virtual s32 Read(void* buf, u32 length);
    virtual bool CanAsync() const;
    virtual bool CanRead() const;
    virtual bool CanWrite() const;
    virtual u32 GetOffsetAlign() const;
    virtual u32 GetSizeAlign() const;
    virtual u32 GetBufferAlign() const;
    virtual u32 GetSize();
    virtual void Seek(s32 offset, ut::SeekOrigin origin);
    virtual void Cancel();
    virtual bool CanSeek() const;
    virtual bool CanCancel() const;
    virtual u32 Tell() const;

    bool Open();

    char path[128];
    DVDFileInfo file;
    u32 start;
    u32 size;
    u32 offset;
    bool cancelled;

    static OSMutex mutex;
    static OSThreadQueue queue;
};
} // namespace snd
} // namespace nw4r
