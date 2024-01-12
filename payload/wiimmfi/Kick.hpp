#include <common/Common.hpp>

namespace Wiimmfi {
namespace Kick {

    // Schedules a kick for the given AID
    void ScheduleForAID(int aid);

    // Schedules a kick for everyone in the room
    void ScheduleForEveryone();

    // Runs the kicking task
    // TODO implement
    void CalcKick();

} // namespace Kick
} // namespace Wiimmfi
