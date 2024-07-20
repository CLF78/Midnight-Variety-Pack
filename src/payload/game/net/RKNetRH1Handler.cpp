#include <common/Common.hpp>
#include <game/net/RKNetController.hpp>
#include <game/net/RKNetRH1Handler.hpp>
#include <game/net/packet/RKNetRH1Packet.hpp>
#include <game/system/RaceConfig.hpp>
#include <mvp/cup/CupCounts.h>
#include <platform/string.h>

///////////////////////////////////////////////
// Custom Cup System / Custom Engine Classes //
///////////////////////////////////////////////

// Replace the course check to get the map from the correct player
REPLACE u8* RKNetRH1Handler::getAidPidMap() {
    for (int i = 0; i < ARRAY_SIZE(datas); i++) {
        if (datas[i].trackId < TRACK_COUNT)
            return datas[i].aidPidMap.playerIds;
    }

    return nullptr;
}

// Replace the course check to get the battle type from the correct player
REPLACE u8 RKNetRH1Handler::getBattleType() {
    for (int i = 0; i < ARRAY_SIZE(datas); i++) {
        if (datas[i].trackId < TRACK_COUNT)
            return datas[i].battleTeamData.battleType;
    }

    return RaceConfig::Settings::BATTLE_INVALID;
}

// Replace the course check to get the engine class from the correct player
REPLACE u8 RKNetRH1Handler::getEngineClass() {
    for (int i = 0; i < ARRAY_SIZE(datas); i++) {
        if (datas[i].trackId < TRACK_COUNT)
            return datas[i].engineClass.raw;
    }

    return 0;
}

// Replace the course check to get the random seed from the correct player
REPLACE u32 RKNetRH1Handler::getRandomSeed() {
    for (int i = 0; i < ARRAY_SIZE(datas); i++) {
        if (datas[i].trackId < TRACK_COUNT)
            return datas[i].randomSeed;
    }

    return 0;
}

// Replace the course check to get the track id from the correct player
REPLACE u32 RKNetRH1Handler::getTrackId() {
    for (int i = 0; i < ARRAY_SIZE(datas); i++) {
        if (datas[i].timer != 0 && datas[i].trackId < TRACK_COUNT)
            return datas[i].trackId;
    }

    return -1;
}

// Patch the course check to get the correct result
REPLACE bool RKNetRH1Handler::isTrackAvailable() {

    // If we are not connected to anyone, bail
    if (!RKNetController::instance->isConnectedToAnyone())
        return false;

    // If nobody has updated RH1, bail
    if (aidsWithUpdatedRH1 == 0)
        return false;

    // Everyone must have updated RH1, if not bail
    RKNetController::Sub* sub = RKNetController::instance->getCurrentSub();
    u32 mask = sub->availableAids & (aidsWithUpdatedRH1 | (1 << sub->myAid));
    if (sub->availableAids != mask)
        return false;

    // Check each data
    for (int i = 0; i < ARRAY_SIZE(datas); i++) {
        if (datas[i].timer != 0 && datas[i].trackId < TRACK_COUNT)
            return true;
    }

    // No one has it
    return false;
}

// Read the correct information from the packet
REPLACE void RKNetRH1Handler::importRecvData() {
    for (int aid = 0; aid < ARRAY_SIZE(datas); aid++) {

        // Skip invalid AIDs
        if (!RKNetController::instance->isConnectedPlayer(aid))
            continue;

        // Get the packet buffer
        // If there is no packet, skip
        RKNetPacketHolder* holder = RKNetController::instance->getPacketRecvBuffer(aid, RKNET_SECTION_RACEHEADER_1);
        if (holder->currentPacketSize == 0)
            continue;

        // Mark AID as available
        u32 aidMask = 1 << aid;
        availableAids |= aidMask;

        // If the packet size doesn't match a RH1 packet, skip
        if (holder->currentPacketSize != sizeof(RKNetRH1Packet))
            continue;

        // Mark AID as updated
        aidsWithUpdatedRH1 |= aidMask;

        // Get the packet and check the player type
        RKNetRH1Packet* packet = (RKNetRH1Packet*)holder->buffer;
        if (packet->playerType == RKNetRH1Packet::PLAYER_UNK_1)
            aidsWithField17At1 |= aidMask;

        // Fill the corresponding data
        RKNetRH1Handler::Data* data = &datas[aid];
        data->timer = packet->frameCount;
        data->vehicles[0] = packet->playerCombos[0].vehicle;
        data->vehicles[1] = packet->playerCombos[1].vehicle;
        data->characters[0] = packet->playerCombos[0].character;
        data->characters[1] = packet->playerCombos[1].character;
        data->trackId = packet->course;
        data->battleTeamData.raw = packet->battleTeamData.raw;
        data->randomSeed = packet->randomSeed;
        memcpy(&data->aidPidMap.raw, &packet->aidPidMap.raw, sizeof(RKNetAidPidMap));
        data->engineClass.raw = packet->engineClass.raw;
        data->starRanks[0] = packet->starRanks[0];
        data->starRanks[1] = packet->starRanks[1];
    }
}

// Set the correct data for sending
REPLACE void RKNetRH1Handler::exportDefaultPacket(u8 aid) {
    RKNetRH1Packet packet(RKNetRH1Packet::PLAYER_UNK_1);
    RKNetPacketHolder* holder = RKNetController::instance->getPacketSendBuffer(aid, RKNET_SECTION_RACEHEADER_1);
    holder->copyData(&packet, sizeof(packet));
}

// Set the correct data for sending
REPLACE void RKNetRH1Handler::exportDefaultSpectatorPacket(u8 aid) {
    RKNetRH1Packet packet(RKNetRH1Packet::PLAYER_SPECTATOR);
    RKNetPacketHolder* holder = RKNetController::instance->getPacketSendBuffer(aid, RKNET_SECTION_RACEHEADER_1);
    holder->copyData(&packet, sizeof(packet));
}

// RKNetRH1Handler::calc() patch
// Patch the course check to get the correct result
kmBranchDefAsm(0x80663EA4, 0x80664084) {
    nofralloc

    // Call C++ code
    mr r3, r31
    CALL_CPP(RKNetRH1Handler::isTrackAvailable())

    // Move result to r0
    mr r0, r3
    blr
}

// RKNetRH1Handler::getTeam() patch
// Patch the course check to get the team from the correct player
// Only doing a patch because the function is confusing as fuck
kmWrite16(0x80664506, TRACK_COUNT);
