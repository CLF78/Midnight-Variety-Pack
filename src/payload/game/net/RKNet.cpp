#include <common/Common.hpp>
#include <game/net/RKNet.hpp>
#include <game/net/RKNetController.hpp>
#include <game/ui/page/FriendRoomJoinPage.hpp>
#include <nw4r/ut/Lock.hpp>
#include <revolutionex/net/NETDigest.h>
#include <wiimmfi/Kick.hpp>
#include <wiimmfi/Security.hpp>

namespace RKNet {

/////////////
// RCE Fix //
/////////////

// Validate incoming player data to prevent remote code execution exploits
// Credits: WiiLink24, Wiimmfi
REPLACE void UserRecvCallback(u32 aid, void* data, u32 dataLength) {

    // Bail if the packet doesn't even include a full header
    if (dataLength < sizeof(RKNetRACEPacketHeader))
        return;

    // Verify the checksum
    // The game already does this later, but we shouldn't disconnect a player because a packet got corrupted
    RKNetRACEPacketHeader* header = (RKNetRACEPacketHeader*)data;
    u32 savedChecksum = header->checksum;
    header->checksum = 0;
    u32 realChecksum = NETCalcCRC32(data, dataLength);
    header->checksum = savedChecksum;
    if (realChecksum != savedChecksum) {
        LOG_DEBUG("[RKNET] Detected corrupted packet from aid %d\n", aid);
        return;
    }

    // If the packet is valid, process it
    if (Wiimmfi::Security::ValidateRACEPacket(aid, header, dataLength)) {
        RKNetController::instance->processRacePacket(aid, data, dataLength);
        return;
    }

    // Lock interrupts
    nw4r::ut::AutoInterruptLock lock;
    LOG_DEBUG("[RKNET] Detected malicious packet from aid %d\n", aid);

    // Kick the offending player if we're host
    if (RKNetController::instance->isPlayerHost())
        Wiimmfi::Kick::ScheduleForAID(aid);

    // Warn the user if possible
    if (FriendRoomJoinPage* page = FriendRoomJoinPage::getPage())
        page->forceConnectionError();
}

} // namespace RKNet
