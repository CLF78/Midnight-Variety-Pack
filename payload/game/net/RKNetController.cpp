#include <common/Common.hpp>
#include <game/net/RKNetController.hpp>
#include <game/ui/SectionManager.hpp>
#include <game/ui/page/FriendRoomJoinPage.hpp>
#include <nw4r/ut/Lock.hpp>
#include <revolutionex/net/NETDigest.h>
#include <wiimmfi/Kick.hpp>
#include <wiimmfi/Natneg.hpp>
#include <wiimmfi/Security.hpp>
#include <wiimmfi/Status.hpp>

/////////////////
// Fast NATNEG //
/////////////////

// RKNetController::mainNetworkLoop() patch
// Update NATNEG
// Credits: Wiimmfi
// TODO can we move this to main wiimmfi loop?
kmCallDefCpp(0x80657990, void, RKNetController* self) {

    // Original call
    self->updateSubsAndVr();

    // Update Wiimmfi NATNEG
    // Since we're in a matching state, force the value to 0
    Wiimmfi::Natneg::Calc(0);
}

/////////////////////
// Network Cleanup //
/////////////////////

// Hook network shutdown to do some cleanup
kmListHookDefCpp(NetShutdownHook) {

    // Delete the Wiimmfi messaging token
    if (Wiimmfi::Status::sToken)
        delete Wiimmfi::Status::sToken;
}

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
    // The game already does this later, but we shouldn't disconnect a player because a the packet got corrupted
    u32 savedChecksum = data->checksum;
    data->checksum = 0;
    u32 realChecksum = NETCalcCRC32(data, dataLength);
    data->checksum = savedChecksum;
    if (realChecksum != savedChecksum)
        return;

    // If the packet is valid, process it
    // Else kick the aid who sent it
    if (Wiimmfi::Security::ValidateRACEPacket(aid, data, dataLength))
        self->processRacePacket(aid, data, dataLength);
    else {
        nw4r::ut::AutoInterruptLock lock;

        // Do not kick players if we're not host
        if (self->isPlayerHost())
            Wiimmfi::Kick::ScheduleForAID(aid);

        // Warn the user if possible
        FriendRoomJoinPage* page = FriendRoomJoinPage::getPage();
        if (page) page->forceConnectionError();
    }
}
