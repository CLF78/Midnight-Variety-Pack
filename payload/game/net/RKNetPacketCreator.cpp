#include <common/Common.hpp>
#include <game/net/RKNetController.hpp>
#include <game/net/RKNetPacketCreator.hpp>
#include <game/net/RKNetUserHandler.hpp>
#include <game/net/packet/RKNetRH1Packet.hpp>
#include <game/system/RaceConfig.hpp>
#include <game/system/RaceManager.hpp>
#include <game/ui/SectionManager.hpp>
#include <midnight/cup/CupManager.hpp>

//////////////////
// Game Lag Fix //
//////////////////

// RKNetPacketCreator::updateAsRacer() patch
// Disable processing of lag frames to fix matrix slowdown and delays caused by slower Dolphin players
// Credits: Wiimmfi
kmWrite32(0x80654400, 0x60000000);

// RKNetPacketCreator::createRH1Racer() override
// Pack the RACEHEADER_1 data
kmBranchDefCpp(0x80654D08, NULL, void, RKNetPacketCreator* self) {
    for (int aid = 0; aid < 12; aid++) {

        // Skip if it's my AID
        RKNetController::Sub* sub = RKNetController::instance->getCurrentSub();
        if (aid == sub->myAid)
            continue;

        // Skip if the AID is not available
        u32 aidMask = 1 << aid;
        if (!(aidMask & sub->availableAids))
            continue;

        // Create the packet
        RKNetRH1Packet packet(RKNetRH1Packet::PLAYER_NORMAL);

        // Start filling data
        RaceConfig::Scenario* scenario = &RaceConfig::instance->raceScenario;
        packet.frameCount = RaceManager::instance->frameCounter;
        packet.randomSeed = scenario->settings.seed1;

        if (scenario->settings.battleType == RaceConfig::Settings::BATTLE_COIN)
            packet.battleTeamData.battleType = RaceConfig::Settings::BATTLE_COIN;

        for (int i = 0; i < 12; i++) {
            if (scenario->players[i].team == RaceConfig::Player::TEAM_RED)
                packet.battleTeamData.teams |= (1 << i);
        }

        packet.course = CupManager::currentSzs;
        packet.engineClass.setEngineClass(scenario->settings.engineClass);
        packet.engineClass.setIsMirrorFlag(scenario->settings.modeFlags);
        packet.aidPidMap = RKNetController::instance->aidPidMap;

        // All the following data is not sent in the spectator version of the function
        if ((1 << sub->myAid) & sub->availableAids)
            packet.lagFrames = self->lagFrames;

        packet.countdownTime = self->countdownTimer - RKNetController::instance->countdownTimers[aid];

        for (int i = 0; i < sub->localPlayerCount; i++) {
            u32 playerIdx = RKNetController::instance->getLocalPlayerIdx(i);
            packet.playerCombos[i].character = scenario->players[playerIdx].characterId;
            packet.playerCombos[i].vehicle = scenario->players[playerIdx].vehicleId;
            packet.starRanks[i] = SectionManager::instance->globalContext->playerCombos[i].starRank;
        }

        // Send the RH1 packet
        RKNetPacketHolder* holder = RKNetController::instance->getPacketSendBuffer(aid, RKNetRACEPacketHeader::RACEHEADER_1);
        holder->copyData(&packet, sizeof(packet));

        // Send the RACEDATA packet
        holder = RKNetController::instance->getPacketSendBuffer(aid, RKNetRACEPacketHeader::RACEDATA);
        holder->copyData(&self->sendRacedataPackets, sizeof(RKNetRACEDATAPacket) * sub->localPlayerCount);

        // Send the USER packet to this AID if they do not have the random seed
        if (!(self->aidsWithSelectId & aidMask))
            RKNetUSERHandler::instance->SetSendUSERPacket(aid);
    }
}

// RKNetPacketCreator::createRH1Spectator() override
// Pack the RACEHEADER_1 data
kmBranchDefCpp(0x80655164, NULL, void, RKNetPacketCreator* self) {
    for (int aid = 0; aid < 12; aid++) {

        // Skip if it's my AID
        RKNetController::Sub* sub = RKNetController::instance->getCurrentSub();
        if (aid == sub->myAid)
            continue;

        // Skip if the AID is not available
        u32 aidMask = 1 << aid;
        if (!(aidMask & sub->availableAids))
            continue;

        // Create the packet
        RKNetRH1Packet packet(RKNetRH1Packet::PLAYER_SPECTATOR);

        // Start filling data
        RaceConfig::Scenario* scenario = &RaceConfig::instance->raceScenario;
        packet.frameCount = RaceManager::instance->frameCounter;
        packet.randomSeed = scenario->settings.seed1;

        if (scenario->settings.battleType == RaceConfig::Settings::BATTLE_COIN)
            packet.battleTeamData.battleType = RaceConfig::Settings::BATTLE_COIN;

        for (int i = 0; i < 12; i++) {
            if (scenario->players[i].team == RaceConfig::Player::TEAM_RED)
                packet.battleTeamData.teams |= (1 << i);
        }

        packet.course = CupManager::currentSzs;
        packet.engineClass.setEngineClass(scenario->settings.engineClass);
        packet.engineClass.setIsMirrorFlag(scenario->settings.modeFlags);
        packet.aidPidMap = RKNetController::instance->aidPidMap;

        // Send the data
        RKNetPacketHolder* holder = RKNetController::instance->getPacketSendBuffer(aid, RKNetRACEPacketHeader::RACEHEADER_1);
        holder->copyData(&packet, sizeof(packet));

        // Send the USER packet to this AID if they do not have the random seed
        if (!(self->aidsWithSelectId & aidMask))
            RKNetUSERHandler::instance->SetSendUSERPacket(aid);
    }
}
