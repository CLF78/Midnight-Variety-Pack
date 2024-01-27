#include <common/Common.hpp>
#include <game/net/RKNetController.hpp>
#include <wiimmfi/Kick.hpp>
#include <wiimmfi/Security.hpp>
#include <wiimmfi/Status.hpp>

/////////////////////
// Wiimmfi Patches //
/////////////////////

// Hook network shutdown to do some cleanup (new code, not ported)
kmListHookDefCpp(NetShutdownHook) {

    // Delete the Wiimmfi messaging token
    if (Wiimmfi::Status::token)
        delete Wiimmfi::Status::token;
}

////////////////////////////
// Wiimmfi Security Fixes //
////////////////////////////

// Fix the RCE bug
kmBranchDefCpp(0x80658610, NULL, void, RKNetController* self, u32 aid, void* data, u32 dataLength) {

    // If the packet is valid, process it
    if (Wiimmfi::Security::ValidatePacket(aid, (RKNetRACEPacketHeader*)data, dataLength))
        self->processRacePacket(aid, data, dataLength);

    // Else kick the aid who sent it
    // TODO page thingy
    else
        Wiimmfi::Kick::ScheduleForAID(aid);
}
