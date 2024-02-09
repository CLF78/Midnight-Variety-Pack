#include <common/Common.hpp>
#include <game/net/RKNetController.hpp>
#include <game/ui/SectionManager.hpp>
#include <game/ui/page/FriendRoomJoinPage.hpp>
#include <nw4r/ut/Lock.hpp>
#include <wiimmfi/Kick.hpp>
#include <wiimmfi/Security.hpp>
#include <wiimmfi/Status.hpp>

/////////////////////
// Network Cleanup //
/////////////////////

// Hook network shutdown to do some cleanup
kmListHookDefCpp(NetShutdownHook) {

    // Delete the Wiimmfi messaging token
    if (Wiimmfi::Status::token)
        delete Wiimmfi::Status::token;
}

/////////////
// RCE Fix //
/////////////

// RKNet_UserRecvCallback() patch
// Validate incoming player data to prevent remote code execution exploits
// Credits: WiiLink24, Wiimmfi
kmBranchDefCpp(0x80658610, NULL, void, RKNetController* self, u32 aid, void* data, u32 dataLength) {

    // If the packet is valid, process it
    if (Wiimmfi::Security::ValidateRACEPacket(aid, (RKNetRACEPacketHeader*)data, dataLength))
        self->processRacePacket(aid, data, dataLength);

    // Else kick the aid who sent it and warn the user if possible
    else {
        nw4r::ut::AutoInterruptLock lock;
        Wiimmfi::Kick::ScheduleForAID(aid);
        FriendRoomJoinPage* page = FriendRoomJoinPage::getPage();
        if (page) page->forceConnectionError();
    }
}
