#include <common/Common.hpp>
#include <game/net/RKNetPacketHolder.hpp>
#include <game/net/RKNetSelectHandler.hpp>
#include <platform/string.h>
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

// RKNetSELECTHandler::PrepareAndExportSELECTAndUSER() patch
// Send the expanded packet
kmCallDefCpp(0x80661040, void, RKNetPacketHolder* holder, RKNetSELECTPacket* data, u32 dataSize) {

    // Copy the original packet
    holder->copyData(data, dataSize);

    // Copy the expansion as well
    holder->appendData(&RKNetSELECTHandler::instance->expansion.sendPacketEx,
                       sizeof(RKNetSELECTPacketExpansion));
}

// RKNetSELECTHandler::ImportNewPackets() patch
// Update incoming packet size check
kmWrite16(0x806610FE, sizeof(RKNetSELECTPacketEx));

// RKNetSelectHandler::ImportNewPackets() patch
// Import the expanded packet correctly
kmHookFn void CopyExpandedPacket(RKNetPacketHolder* holder, u32 aid) {

    // Copy the expansion data
    void* dest = &RKNetSELECTHandler::instance->expansion.recvPacketsEx[aid];
    memcpy(dest, ((u8*)holder->buffer) + sizeof(RKNetSELECTPacket), sizeof(RKNetSELECTPacketExpansion));

    // Clear the buffer (original call)
    holder->clear();
}

// Glue code
kmCallDefAsm(0x806611D4) {
    nofralloc

    // Call C++ code
    mr r4, r19
    b CopyExpandedPacket
    blr
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
