#include <kamek.h>
#include <rvl/os/OSThread.h>

#ifdef __cplusplus
extern "C" {
#endif

struct OSMutex {
    OSThreadQueue queue;
    OSThread* thread;
    s32 count;
    OSMutexLink link;
};
size_assert(OSMutex, 0x18);

void OSLockMutex(OSMutex* mutex);
void OSUnlockMutex(OSMutex* mutex);
bool OSTryLockMutex(OSMutex* mutex);

#ifdef __cplusplus
}
#endif
