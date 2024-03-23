#include <common/Common.hpp>
#include <game/net/RKNetSelectHandler.hpp>
#include <wiimmfi/RoomStall.hpp>

///////////////////////////////////////////////
// Custom Cup System / Custom Engine Classes //
///////////////////////////////////////////////

// RKNetSELECTHandler::getStaticInstance() patch
// Update size of class
kmCallDefCpp(0x8065FEB0, u32) {
    return sizeof(RKNetSELECTHandler);
}

// RKNetSELECTHandler::getStaticInstance() patch
// Construct the expansion data
kmCallDefCpp(0x8065FF48, void, RKNetSELECTHandler* self) {

    // Store instance
    RKNetSELECTHandler::instance = self;

    // Construct expansion data
    if (self) RKNetSELECTHandlerEx::construct(&self->expansion);
}

//////////////////////////////////
// Online Room Stall Prevention //
//////////////////////////////////

// RKNetSELECTHandler::calc() patch
// Update the timer every frame and kick if necessary
// Credits: Wiimmfi
kmCallDefCpp(0x80660330, bool) {
    Wiimmfi::RoomStall::Update();
    return true; // Not calling the original function here because the result would always be true
}

// RKNetSELECTHandler::getStaticInstance() patch
// Initialize the timer
// Credits: Wiimmfi
kmBranchDefCpp(0x8065FF5C, NULL, void) {
    Wiimmfi::RoomStall::Init();
}
