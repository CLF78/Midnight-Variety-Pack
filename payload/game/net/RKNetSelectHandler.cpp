#include <common/Common.hpp>
#include <game/net/RKNetSelectHandler.hpp>
#include <wiimmfi/RoomStall.hpp>

///////////////////////////////////
// Wiimmfi Room Stall Prevention //
///////////////////////////////////

// Reset the timer on SELECT creation
kmBranch(0x8065FF5C, Wiimmfi::RoomStall::Reset);

// Update the timer every frame and kick if necessary
kmCallDefCpp(0x80660330, bool) {
    Wiimmfi::RoomStall::Update();
    return true; // I'm not calling the original function here because the result would always be true
}
