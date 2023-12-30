#include <nw4r/snd/InstancePool.hpp>
#include <nw4r/ut/utList.hpp>
#include <revolution/os/OSMutex.h>

namespace nw4r {
namespace snd {
namespace detail {

class SoundInstanceManager {
    public:
        PoolImpl pool;
        ut::LinkListImpl priorityList;
        OSMutex mutex;
};
size_assert(SoundInstanceManager, 0x28);

} // namespace detail
} // namespace snd
} // namespace nw4r
