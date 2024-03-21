#include <common/Common.hpp>
#include <game/net/RKNetController.hpp>
#include <game/net/RKNetRH1Handler.hpp>
#include <game/net/packet/RKNetRH1Packet.hpp>
#include <game/system/RaceConfig.hpp>
#include <midnight/cup/CupCounts.h>
#include <platform/string.h>

///////////////////////
// Custom Cup System //
///////////////////////

kmHookFn bool IsTrackAvailable(RKNetRH1Handler* self) {

    // If we are not connected to anyone, bail
    if (!RKNetController::instance->isConnectedToAnyone())
        return false;

    // If nobody has updated RH1, bail
    if (self->aidsWithUpdatedRH1 == 0)
        return false;

    // Everyone must have updated RH1, if not bail
    RKNetController::Sub* sub = RKNetController::instance->getCurrentSub();
    u32 mask = sub->availableAids & (self->aidsWithUpdatedRH1 | (1 << sub->myAid));
    if (sub->availableAids != mask)
        return false;

    // Check each data
    for (int i = 0; i < ARRAY_SIZE(self->datas); i++) {
        if (self->datas[i].timer != 0 && self->datas[i].trackId < TRACK_COUNT)
            return true;
    }

    // No one has it
    return false;
}

// RKNetRH1Handler::calc() patch
// Patch the course check to get the correct result
kmBranchDefAsm(0x80663EA4, 0x80664084) {
    nofralloc

    // Call C++ code
    mr r3, r31
    bl IsTrackAvailable

    // Move result to r0
    mr r0, r3
    blr
}

// RKNetRH1Handler::isTrackAvailable() override
// Patch the course check to get the correct result
kmBranch(0x806641B0, IsTrackAvailable);

// RKNetRH1Handler::getTeam() patch
// Patch the course check to get the team from the correct player
// Only doing a patch because the function is confusing as fuck
kmWrite16(0x80664506, TRACK_COUNT);

// RKNetRH1Handler::getTrackId() override
// Replace the course check to get the track id from the correct player
kmBranchDefCpp(0x80664560, NULL, u32, RKNetRH1Handler* self) {

    for (int i = 0; i < ARRAY_SIZE(self->datas); i++) {
        if (self->datas[i].timer != 0 && self->datas[i].trackId < TRACK_COUNT)
            return self->datas[i].trackId;
    }

    return -1;
}

// RKNetRH1Handler::getBattleType() override
// Replace the course check to get the battle type from the correct player
kmBranchDefCpp(0x806646C8, NULL, u8, RKNetRH1Handler* self) {

    for (int i = 0; i < ARRAY_SIZE(self->datas); i++) {
        if (self->datas[i].trackId < TRACK_COUNT)
            return self->datas[i].battleTeamData.battleType;
    }

    return RaceConfig::Settings::BATTLE_INVALID;
}

// RKNetRH1Handler::getRandomSeed() override
// Replace the course check to get the random seed from the correct player
kmBranchDefCpp(0x80664944, NULL, u32, RKNetRH1Handler* self) {

    for (int i = 0; i < ARRAY_SIZE(self->datas); i++) {
        if (self->datas[i].trackId < TRACK_COUNT)
            return self->datas[i].randomSeed;
    }

    return 0;
}

// RKNetRH1Handler::getEngineClass() override
// Replace the course check to get the engine class from the correct player
kmBranchDefCpp(0x80664A3C, NULL, u8, RKNetRH1Handler* self) {

    for (int i = 0; i < ARRAY_SIZE(self->datas); i++) {
        if (self->datas[i].trackId < TRACK_COUNT)
            return self->datas[i].engineClass.raw;
    }

    return RaceConfig::Settings::CC_50;
}

// RKNetRH1Handler::getAidPidMap() override
// Replace the course check to get the map from the correct player
kmBranchDefCpp(0x80664B34, NULL, u8*, RKNetRH1Handler* self) {

    for (int i = 0; i < ARRAY_SIZE(self->datas); i++) {
        if (self->datas[i].trackId < TRACK_COUNT)
            return self->datas[i].aidPidMap.playerIds;
    }

    return nullptr;
}

// RKNetRH1Handler::importRecvData() override
// Read the correct information from the packet
kmBranchDefCpp(0x806651B4, NULL, void, RKNetRH1Handler* self) {

    // Import info for every AID
    for (int aid = 0; aid < ARRAY_SIZE(self->datas); aid++) {

        // Skip if it's my AID
        RKNetController::Sub* sub = RKNetController::instance->getCurrentSub();
        if (aid == sub->myAid)
            continue;

        // Skip if the AID is not available
        u32 aidMask = 1 << aid;
        if (!(aidMask & sub->availableAids))
            continue;

        // Get the packet buffer
        // If there is no packet, skip
        RKNetPacketHolder* holder = RKNetController::instance->getPacketRecvBuffer(aid, RKNetRACEPacketHeader::RACEHEADER_1);
        if (holder->currentPacketSize == 0)
            continue;

        // Mark AID as available
        self->availableAids |= aidMask;

        // If the packet size doesn't match a RH1 packet, skip
        if (holder->currentPacketSize != sizeof(RKNetRH1Packet))
            continue;

        // Mark AID as updated
        self->aidsWithUpdatedRH1 |= aidMask;

        // Get the packet and check the player type
        RKNetRH1Packet* packet = (RKNetRH1Packet*)holder->buffer;
        if (packet->playerType == RKNetRH1Packet::PLAYER_UNK_1)
            self->aidsWithField17At1 |= aidMask;

        // Fill the corresponding data
        RKNetRH1Handler::Data* data = &self->datas[aid];
        data->timer = packet->frameCount;
        data->vehicles[0] = packet->player1Vehicle;
        data->vehicles[1] = packet->player2Vehicle;
        data->characters[0] = packet->player1Character;
        data->characters[1] = packet->player2Character;
        data->trackId = packet->course;
        data->battleTeamData.raw = packet->battleTeamData.raw;
        data->randomSeed = packet->randomSeed;
        memcpy(&data->aidPidMap.raw, &packet->aidPidMap.raw, sizeof(RKNetAidPidMap));
        data->engineClass.raw = packet->engineClass.raw;
        data->starRanks[0] = packet->starRanks[0];
        data->starRanks[1] = packet->starRanks[1];
    }
}

// RKNetRH1Handler::exportDefaultPacket() override
// Set the correct data for sending
kmBranchDefCpp(0x806653C8, NULL, void, RKNetRH1Handler* self, u8 aid) {

    RKNetRH1Packet packet(RKNetRH1Packet::PLAYER_UNK_1);
    RKNetPacketHolder* holder = RKNetController::instance->getPacketSendBuffer(aid, RKNetRACEPacketHeader::RACEHEADER_1);
    holder->copyData(&packet, sizeof(packet));
}

// RKNetRH1Handler::exportDefaultSpectatorPacket() override
// Set the correct data for sending
kmBranchDefCpp(0x80665480, NULL, void, RKNetRH1Handler* self, u8 aid) {

    RKNetRH1Packet packet(RKNetRH1Packet::PLAYER_SPECTATOR);
    RKNetPacketHolder* holder = RKNetController::instance->getPacketSendBuffer(aid, RKNetRACEPacketHeader::RACEHEADER_1);
    holder->copyData(&packet, sizeof(packet));
}
