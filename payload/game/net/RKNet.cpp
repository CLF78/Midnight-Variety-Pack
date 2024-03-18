#include <common/Common.hpp>
#include <game/net/RKNetController.hpp>
#include <game/ui/page/FriendRoomJoinPage.hpp>
#include <nw4r/ut/Lock.hpp>
#include <revolutionex/net/NETDigest.h>
#include <wiimmfi/Kick.hpp>
#include <wiimmfi/Security.hpp>

/////////////
// RCE Fix //
/////////////

// RKNet_UserRecvCallback() patch
// Validate incoming player data to prevent remote code execution exploits
// Credits: WiiLink24, Wiimmfi
kmBranchDefCpp(0x80658610, NULL, void, RKNetController* self, u32 aid, RKNetRACEPacketHeader* data, u32 dataLength) {

    // Bail if the packet doesn't even include a full header
    if (dataLength < sizeof(RKNetRACEPacketHeader))
        return;

    // Verify the checksum
    // The game already does this later, but we shouldn't disconnect a player because a packet got corrupted
    u32 savedChecksum = data->checksum;
    data->checksum = 0;
    u32 realChecksum = NETCalcCRC32(data, dataLength);
    data->checksum = savedChecksum;
    if (realChecksum != savedChecksum) {
        DEBUG_REPORT("[RKNET] Detected corrupted packet from aid %d\n", aid)
        return;
    }

    // If the packet is valid, process it
    // Else kick the aid who sent it
    if (Wiimmfi::Security::ValidateRACEPacket(aid, data, dataLength))
        self->processRacePacket(aid, data, dataLength);
    else {
        nw4r::ut::AutoInterruptLock lock;
        DEBUG_REPORT("[RKNET] Detected malicious packet from aid %d\n", aid)

        // Do not kick players if we're not host
        if (self->isPlayerHost())
            Wiimmfi::Kick::ScheduleForAID(aid);

        // Warn the user if possible
        FriendRoomJoinPage* page = FriendRoomJoinPage::getPage();
        if (page) page->forceConnectionError();
    }
}
