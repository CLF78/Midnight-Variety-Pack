#include <common/Common.hpp>
#include <dwc/dwc_friend.h>
#include <dwc/dwc_match.h>
#include <dwc/dwc_transport.h>
#include <game/net/packet/RKNetEventPacket.hpp>
#include <game/net/packet/RKNetItemPacket.hpp>
#include <game/net/packet/RKNetRacedataPacket.hpp>
#include <game/net/packet/RKNetRH1Packet.hpp>
#include <game/net/packet/RKNetRH2Packet.hpp>
#include <game/net/packet/RKNetUserPacket.hpp>
#include <game/net/RKNetController.hpp>
#include <game/net/RKNetSelectHandler.hpp>
#include <game/race/RaceGlobals.hpp>
#include <game/system/RaceManager.hpp>
#include <game/ui/SectionManager.hpp>
#include <game/ui/page/FriendRoomJoinPage.hpp>
#include <nw4r/ut/Lock.hpp>
#include <platform/new.hpp>
#include <platform/string.h>
#include <revolution/vi.h>
#include <revolutionex/net/NETDigest.h>
#include <wiimmfi/Delay.hpp>
#include <wiimmfi/Kick.hpp>
#include <wiimmfi/Natneg.hpp>
#include <wiimmfi/Reporting.hpp>
#include <wiimmfi/Security.hpp>
#include <wiimmfi/Status.hpp>

/////////////////////
// Debug Reporting //
/////////////////////

// RKNetController::Connect() patch
// Print DWC debug messages to the console
#if DEBUG
kmWrite16(0x80658BE6, 0xFFFF);
#endif

/////////////////
// Fast NATNEG //
/////////////////

// RKNetController::mainNetworkLoop() patch
// Update NATNEG
// Credits: Wiimmfi
// Q: Can we move this to main Wiimmfi network loop?
kmCallDefCpp(0x80657990, void, RKNetController* self) {

    // Original call
    self->updateSubsAndVr();

    // Update Wiimmfi NATNEG
    // Since we're in a matching state, force the value to 0
    Wiimmfi::Natneg::CalcTimers(false);
}

////////////////////////////////
// Network Protocol Expansion //
////////////////////////////////

const u32 packetbufferSizes[8] = {
    sizeof(RKNetRACEPacketHeader),
    sizeof(RKNetRH1Packet),
    sizeof(RKNetRH2Packet),
    MAX(sizeof(RKNetROOMPacket), sizeof(RKNetSELECTPacket)),
    sizeof(RKNetRACEDATAPacket) * 2,
    sizeof(RKNetUSERPacket),
    sizeof(RKNetITEMPacket) * 2,
    sizeof(RKNetEVENTPacket),
};

const u32 totalPacketBufSize = sizeof(RKNetRACEPacketHeader) + sizeof(RKNetRH1Packet) + sizeof(RKNetRH2Packet)
                             + MAX(sizeof(RKNetROOMPacket), sizeof(RKNetSELECTPacket)) + sizeof(RKNetRACEDATAPacket) * 2
                             + sizeof(RKNetUSERPacket) + sizeof(RKNetITEMPacket) * 2 + sizeof(RKNetEVENTPacket);

// Replace the individual packet section sizes
kmWriteArea(0x8089A194, packetbufferSizes);

// RKNetController::RKNetController() patches
// Replace the packet buffer size
kmWrite16(0x80657086, totalPacketBufSize);
kmWrite16(0x806570B2, totalPacketBufSize);

// RKNetController::RKNetController() patch
// Move the received packet buffers outside the class to allow different packet sizes
kmBranchDefCpp(0x806573D8, NULL, RKNetController*, RKNetController* self) {

    // Create the buffers
    for (int i = 0; i < 12; i++)
        self->fullRecvRACEPackets[i] = new (self->heap, 4) u8[totalPacketBufSize];

    // Return self since we're hooking a constructor
    return self;
}

// RKNetController::Connect() patch
// Set the custom buffers for receiving
kmBranchDefCpp(0x80658C68, 0x80658CA4, void, DWCBuddyFriendCallback cb, RKNetController* self) {

    // Original call
    DWC_SetBuddyFriendCallback(cb, self);

    // Clear the receiving buffers and set them
    for (int i = 0; i < 12; i++) {
        memset(self->fullRecvRACEPackets[i], 0, totalPacketBufSize);
        DWC_SetRecvBuffer(i, self->fullRecvRACEPackets[i], totalPacketBufSize);
    }
}

// RKNetController::StartThread() patch
// Clear the custom buffers
kmCallDefCpp(0x8065607C, void, RKNetController* self) {
    for (int i = 0; i < 12; i++)
        memset(self->fullRecvRACEPackets[i], 0, totalPacketBufSize);
}

// Glue code
kmWrite16(0x8065606E, 0);

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

/////////////////////////////////
// Wiimmfi Telemetry / Various //
/////////////////////////////////

// RKNetController::mainNetworkLoop() patch
// Main Wiimmfi update function
// Credits: Wiimmfi
kmCallDefCpp(0x806579B0, void) {

    // Only run the tasks if we are online
    if (stpMatchCnt) {
        nw4r::ut::AutoInterruptLock lock;
        Wiimmfi::Kick::CalcKick();
        Wiimmfi::Reporting::ReportMatchStateChange();
        Wiimmfi::Natneg::StopNATNEGAfterTime();
        Wiimmfi::Reporting::ReportSuspendUpdate();
        Wiimmfi::Reporting::ReportAIDPIDMap();
        Wiimmfi::Reporting::ReportHostSlotChange();
        Wiimmfi::Reporting::ReportRegionChange();

        // Only run race tasks if we are in an online race
        if (RaceGlobals::isOnlineRace && RaceManager::instance != nullptr) {
            Wiimmfi::Reporting::ReportFrameCount(RaceManager::instance->frameCounter);
            Wiimmfi::Delay::Calc(RaceManager::instance->frameCounter);
        }

        // Only run SELECT-tasks if the handler exists
        if (RKNetSELECTHandler::instance)
            Wiimmfi::Reporting::ReportSELECTInfo();
    }

    // Original call
    VIWaitForRetrace();
}
