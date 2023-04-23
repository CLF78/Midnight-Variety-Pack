#include <kamek.h>
#include <rvl/os/OS.h>
#include <rvl/os/OSMutex.h>

namespace nw4r {
namespace ut {

class AutoMutexLock {
public:
    AutoMutexLock(OSMutex& mutex) : mutex(mutex) { OSLockMutex(&mutex); }
    ~AutoMutexLock() { OSUnlockMutex(&mutex); }

    OSMutex& mutex;
};

class AutoInterruptLock {
public:
    AutoInterruptLock() : interruptState(OSDisableInterrupts()) {}
    ~AutoInterruptLock() { OSRestoreInterrupts(interruptState); }

    bool interruptState;
};

} // namespace ut
} // namespace nw4r
