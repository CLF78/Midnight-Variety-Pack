#include <common/Common.hpp>
#include <game/net/RKNetController.hpp>
#include <game/net/packet/RKNetRH1Packet.hpp>
#include <game/system/RaceConfig.hpp>
#include <game/system/RaceManager.hpp>
#include <midnight/cup/CupManager.hpp>

//////////////////
// Game Lag Fix //
//////////////////

// RKNetPacketCreator::updateAsRacer() patch
// Disable processing of lag frames to fix matrix slowdown and delays caused by slower Dolphin players
// Credits: Wiimmfi
kmWrite32(0x80654400, 0x60000000);

// RKNetPacketCreator::createRH1Spectator() override
// Pack the RACEHEADER_1 data
// TODO packet creator stuff
kmBranchDefCpp(0x80655164, NULL, void, void* self) {

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

        // TODO send user packet
    }
}
