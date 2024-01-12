#include <common/Common.hpp>
#include <wiimmfi/Kick.hpp>

namespace Wiimmfi {
namespace Kick {

u32 aidsToBeKicked;

void ScheduleForAID(int aid) {
    aidsToBeKicked |= (1 << aid);
}

void ScheduleForEveryone() {
    aidsToBeKicked = 0xFFFFFFFF;
}

} // namespace Kick
} // namespace Wiimmfi
