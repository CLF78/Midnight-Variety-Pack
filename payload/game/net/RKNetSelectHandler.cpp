#include <common/Common.hpp>
#include <game/net/RKNetSelectHandler.hpp>
#include <wiimmfi/RoomStall.hpp>

//////////////////////////////////
// Online Room Stall Prevention //
//////////////////////////////////

// RKNetSELECTHandler::getStaticInstance() patch
// Reset the timer on SELECT creation
// Credits: Wiimmfi
kmBranch(0x8065FF5C, Wiimmfi::RoomStall::Reset);

// RKNetSELECTHandler::calc() patch
// Update the timer every frame and kick if necessary
// Credits: Wiimmfi
kmCallDefCpp(0x80660330, bool) {
    Wiimmfi::RoomStall::Update();
    return true; // Not calling the original function here because the result would always be true
}
