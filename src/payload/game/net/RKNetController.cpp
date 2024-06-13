#include <common/Common.hpp>
#include <dwc/dwc_friend.h>
#include <dwc/dwc_match.h>
#include <dwc/dwc_transport.h>
#include <game/net/packet/RKNetRacePacketHeader.hpp>
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
#include <nw4r/ut/Lock.hpp>
#include <platform/string.h>
#include <revolution/vi.h>
#include <wiimmfi/Delay.hpp>
#include <wiimmfi/Kick.hpp>
#include <wiimmfi/Natneg.hpp>
#include <wiimmfi/Reporting.hpp>
#include <wiimmfi/Status.hpp>

////////////////////////////////
// Network Protocol Expansion //
////////////////////////////////

const u32 packetbufferSizes[RKNET_SECTION_COUNT] = {
    sizeof(RKNetRACEPacketHeader),
    sizeof(RKNetRH1Packet),
    sizeof(RKNetRH2Packet),
    MAX(sizeof(RKNetROOMPacket), sizeof(RKNetSELECTPacketEx)),
    sizeof(RKNetRACEDATAPacket) * 2,
    sizeof(RKNetUSERPacket),
    sizeof(RKNetITEMPacket) * 2,
    sizeof(RKNetEVENTPacket),
};

const u32 totalPacketBufSize = sizeof(RKNetRACEPacketHeader) + sizeof(RKNetRH1Packet) + sizeof(RKNetRH2Packet)
                             + MAX(sizeof(RKNetROOMPacket), sizeof(RKNetSELECTPacketEx)) + sizeof(RKNetRACEDATAPacket) * 2
                             + sizeof(RKNetUSERPacket) + sizeof(RKNetITEMPacket) * 2 + sizeof(RKNetEVENTPacket);

// Replace the individual packet section sizes
kmWriteArea(0x8089A194, packetbufferSizes);

// Move the received packet buffers outside the class to allow different packet sizes
REPLACE RKNetController::RKNetController(EGG::Heap* heap) {

    // Create the class
    REPLACED(heap);

    // Create the buffers
    for (int i = 0; i < 12; i++)
        fullRecvRACEPackets[i] = new (this->heap, 4) u8[totalPacketBufSize];
}

// RKNetController::RKNetController() patches
// Replace the total packet buffer size
kmWrite16(0x80657086, totalPacketBufSize);
kmWrite16(0x806570B2, totalPacketBufSize);

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

        // Only run matching tasks if we are in a matching state
        if (RKNetController::instance->connState == RKNetController::STATE_MATCHING)
            Wiimmfi::Natneg::CalcTimers(false);

        // Run other tasks
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

        // Only run SELECT tasks if the handler exists
        if (RKNetSELECTHandler::instance)
            Wiimmfi::Reporting::ReportSELECTInfo();
    }

    // Original call
    VIWaitForRetrace();
}
