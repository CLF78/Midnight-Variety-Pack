#include <kamek.h>
#include <nw4r/ut/FileStream.h>
#include <rvl/dvd/dvd.h>
#include <rvl/os/OSMutex.h>
#include <rvl/os/OSThread.h>

namespace nw4r {
namespace snd {

class DVDFileStream : public ut::FileStream {
public:
    DVDFileStream(const char *path);
    DVDFileStream(DVDFileInfo* file, u32 start, u32 size);

    virtual ~DVDFileStream();
    virtual void Close();
    virtual s32 Read(void *buf, u32 length);
    virtual bool CanAsync();
    virtual bool CanRead();
    virtual bool CanWrite();
    virtual u32 GetOffsetAlign();
    virtual u32 GetSizeAlign();
    virtual u32 GetBufferAlign();
    virtual u32 GetSize();
    virtual void Seek(s32 offset, ut::SeekOrigin origin);
    virtual void Cancel();
    virtual bool CanSeek();
    virtual bool CanCancel();
    virtual u32 Tell();

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
