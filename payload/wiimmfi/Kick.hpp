#include <common/Common.hpp>
#include <gs/gp/gp.h>

namespace Wiimmfi {
namespace Kick {

    enum Type {
        EVERYONE = 1,
        SELF,
        END_RACE,
        SPECIFIC_PLAYER,
    };

    const char KICK_MSG[] = "\\wiimmfi\\kick";
    const char KICK_MSG_PARAM_PID[] = "\\kickpid\\";

    // Schedules a kick for the given AID
    void ScheduleForAID(int aid);

    // Schedules a kick for everyone in the room
    void ScheduleForEveryone();

    // Runs the kicking task
    void CalcKick();

    // Parses a kick message received from the server
    int ParseKickMessage(GPConnection conn, char* data);

} // namespace Kick
} // namespace Wiimmfi
