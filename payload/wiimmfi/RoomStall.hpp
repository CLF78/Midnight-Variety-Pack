#include <common/Common.hpp>

namespace Wiimmfi {
namespace RoomStall {

    ///////////////
    // Constants //
    ///////////////

    const int KICK_THRESHOLD_TIME = 90 * 60;

    ///////////////
    // Functions //
    ///////////////

    // Resets the room stall timer
    void Reset();

    // Updates the room stall timer and kicks players if necessary
    void Update();

    ///////////////
    // Variables //
    ///////////////

    // The kick timer
    extern u16 kickTimer;

} // namespace RoomStall
} // namespace Wiimmfi
