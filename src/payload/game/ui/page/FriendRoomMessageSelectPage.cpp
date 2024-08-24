#include <game/net/RKNetRoomHandler.hpp>
#include <wiimmfi/Reporting.hpp>

///////////////////////
// Wiimmfi Telemetry //
///////////////////////

// FriendRoomMessageSelectPage::onModeBtnClick() patch
// Report friend room starts
kmCallDefCpp(0x805DCE34, void, RKNetROOMHandler* self, u32 data) {

    // Call telemetry function
    RKNetROOMPacket packet(data);
    Wiimmfi::Reporting::ReportFriendRoomStart(&packet);

    // Original call
    self->SetAllSendPackets(data);
}
