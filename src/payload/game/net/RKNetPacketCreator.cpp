#include "RKNetController.hpp"
#include "RKNetPacketCreator.hpp"
#include "RKNetUserHandler.hpp"
#include "packet/RKNetRH1Packet.hpp"
#include <game/system/RaceConfig.hpp>
#include <game/system/RaceManager.hpp>
#include <game/ui/SectionManager.hpp>
#include <mvp/cup/CupManager.hpp>

///////////////////////////////////////////////
// Custom Cup System / Custom Engine Classes //
///////////////////////////////////////////////

// Pack the RACEHEADER_1 data and send it (for players in the race)
REPLACE void RKNetPacketCreator::createRH1Racer() {
    for (int aid = 0; aid < 12; aid++) {

        // Skip invalid AIDs
        if (!RKNetController::instance->isConnectedPlayer(aid))
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
        const RKNetController::Sub* sub = RKNetController::instance->getCurrentSub();
        if ((1 << sub->myAid) & sub->availableAids)
            packet.lagFrames = lagFrames;

        packet.countdownTime = countdownTimer - RKNetController::instance->countdownTimers[aid];

        for (int i = 0; i < sub->localPlayerCount; i++) {
            const u32 playerIdx = RKNetController::instance->getLocalPlayerIdx(i);
            packet.playerCombos[i].character = scenario->players[playerIdx].characterId;
            packet.playerCombos[i].vehicle = scenario->players[playerIdx].vehicleId;
            packet.starRanks[i] = SectionManager::instance->globalContext->playerCombos[i].starRank;
        }

        // Send the RH1 packet
        RKNetPacketHolder* holder = RKNetController::instance->getPacketSendBuffer(aid, RKNET_SECTION_RACEHEADER_1);
        holder->copyData(&packet, sizeof(packet));

        // Send the RACEDATA packet
        holder = RKNetController::instance->getPacketSendBuffer(aid, RKNET_SECTION_RACEDATA);
        holder->copyData(&sendRacedataPackets, sizeof(RKNetRACEDATAPacket) * sub->localPlayerCount);

        // Send the USER packet to this AID if they do not have the random seed
        if (!(aidsWithSelectId & (1 << aid)))
            RKNetUSERHandler::instance->SetSendUSERPacket(aid);
    }
}

// Pack the RACEHEADER_1 data and send it (for spectators)
REPLACE void RKNetPacketCreator::createRH1Spectator() {
    for (int aid = 0; aid < 12; aid++) {

        // Skip invalid AIDs
        if (!RKNetController::instance->isConnectedPlayer(aid))
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
        RKNetPacketHolder* holder = RKNetController::instance->getPacketSendBuffer(aid, RKNET_SECTION_RACEHEADER_1);
        holder->copyData(&packet, sizeof(packet));

        // Send the USER packet to this AID if they do not have the random seed
        if (!(aidsWithSelectId & (1 << aid)))
            RKNetUSERHandler::instance->SetSendUSERPacket(aid);
    }
}

/////////////////////////
// Matrix Slowdown Fix //
/////////////////////////

// RKNetPacketCreator::updateAsRacer() patch
// Disable processing of lag frames to fix matrix slowdown and delays caused by slower Dolphin players
// Credits: Wiimmfi
kmWrite32(0x80654400, 0x60000000);
