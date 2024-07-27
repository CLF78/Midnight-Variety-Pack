#include <common/Common.hpp>
#include <game/net/RKNetController.hpp>
#include <game/net/RKNetSelectHandler.hpp>
#include <game/net/RKNetUserHandler.hpp>
#include <game/system/SystemManager.hpp>
#include <game/ui/page/VotingBackPage.hpp>
#include <game/util/Random.hpp>
#include <mvp/cup/CupManager.hpp>

///////////////////////
// Always Show Timer //
///////////////////////

// VotingBackPage::beforeCalc() patch
// Always display the timer on the course voting screen
kmWrite32(0x80650254, 0x60000000);

///////////////////
// Course Voting //
///////////////////

// Get the instruction text
// TODO figure out what to do here
u32 VotingBackPage::getInstructionText(MessageInfo* msgInfo) {
    return getInstructionText();
}

// Set up the race with the custom settings
REPLACE void VotingBackPage::setupRace() {

    // Define useful variables
    RaceConfig* rconf = RaceConfig::instance;
    RaceConfig::Scenario* scenario = &rconf->menuScenario;
    RaceConfig::Settings* settings = &scenario->settings;
    GlobalContext* ctx = SectionManager::instance->globalContext;
    u32 curSection = SectionManager::instance->curSection->sectionId;
    RKNetSELECTHandler* selectHandler = RKNetSELECTHandler::instance;

    // Update the AID to PID map
    RKNetController::instance->updateAidPidMap();

    // Initialize the race scenario
    if (ctx->friendRoomRaceNumber >= 1) {
        rconf->update();
    } else {
        rconf->reset();
    }

    // Set shared random seed and camera mode
    settings->seed1 = selectHandler->getRandomSeed();
    settings->cameraMode = RaceConfig::Settings::CAMERA_MODE_GAMEPLAY_NO_INTRO;

    // Set the track
    // If random, pick a variant using the shared seed
    u32 actualTrackIdx = CupManager::getTrackFile(selectHandler->getWinningTrack(), &settings->seed1);
    CupManager::SetCourse(settings, actualTrackIdx);

    // Set game mode depending on the current menu
    switch (curSection) {
        case Section::WIFI_VS_1P_VOTE:
        case Section::WIFI_VS_2P_VOTE:
            settings->gameMode = RaceConfig::Settings::GAMEMODE_PUBLIC_VS;
            break;

        case Section::WIFI_BT_1P_VOTE:
        case Section::WIFI_BT_2P_VOTE:
            settings->gameMode = RaceConfig::Settings::GAMEMODE_PUBLIC_BT;
            break;

        case Section::WIFI_FROOM_1P_VS_VOTE:
        case Section::WIFI_FROOM_2P_VS_VOTE:
        case Section::WIFI_FROOM_1P_TEAM_VS_VOTE:
        case Section::WIFI_FROOM_2P_TEAM_VS_VOTE:
            settings->gameMode = RaceConfig::Settings::GAMEMODE_PRIVATE_VS;
            break;

        case Section::WIFI_FROOM_1P_BALLOON_BATTLE_VOTE:
        case Section::WIFI_FROOM_2P_BALLOON_BATTLE_VOTE:
        case Section::WIFI_FROOM_1P_COIN_RUNNERS_VOTE:
        case Section::WIFI_FROOM_2P_COIN_RUNNERS_VOTE:
            settings->gameMode = RaceConfig::Settings::GAMEMODE_PRIVATE_BT;
            break;
    }

    // Set teams mode depending on the current menu
    switch (curSection) {
        case Section::WIFI_VS_1P_VOTE:
        case Section::WIFI_VS_2P_VOTE:
        case Section::WIFI_FROOM_1P_VS_VOTE:
        case Section::WIFI_FROOM_2P_VS_VOTE:
            settings->modeFlags &= ~RaceConfig::Settings::FLAG_TEAMS;
            break;

        case Section::WIFI_BT_1P_VOTE:
        case Section::WIFI_BT_2P_VOTE:
        case Section::WIFI_FROOM_1P_TEAM_VS_VOTE:
        case Section::WIFI_FROOM_2P_TEAM_VS_VOTE:
        case Section::WIFI_FROOM_1P_BALLOON_BATTLE_VOTE:
        case Section::WIFI_FROOM_2P_BALLOON_BATTLE_VOTE:
        case Section::WIFI_FROOM_1P_COIN_RUNNERS_VOTE:
        case Section::WIFI_FROOM_2P_COIN_RUNNERS_VOTE:
            settings->modeFlags |= RaceConfig::Settings::FLAG_TEAMS;
            break;
    }

    // Set engine class and the mirror flag
    RKNetEngineClassData engineClassData(engineClass);
    settings->engineClass = engineClassData.getEngineClass();
    settings->modeFlags |= engineClassData.getIsMirrorFlag();

    // Set battle type depending on the current menu (if applicable)
    switch (curSection) {
        case Section::WIFI_BT_1P_VOTE:
        case Section::WIFI_BT_2P_VOTE:
            settings->battleType = battleType;
            break;

        case Section::WIFI_FROOM_1P_BALLOON_BATTLE_VOTE:
        case Section::WIFI_FROOM_2P_BALLOON_BATTLE_VOTE:
            settings->battleType = RaceConfig::Settings::BATTLE_BALLOON;
            break;

        case Section::WIFI_FROOM_1P_COIN_RUNNERS_VOTE:
        case Section::WIFI_FROOM_2P_COIN_RUNNERS_VOTE:
            settings->battleType = RaceConfig::Settings::BATTLE_COIN;
            break;
    }

    // Start player setup loop
    for (int playerId = 0; playerId < 12; playerId++) {

        // Get the aid of the player
        u8 aid = RKNetController::instance->aidPidMap.playerIds[playerId];
        bool isLocal = aid == RKNetController::instance->getCurrentSub()->myAid;
        RaceConfig::Player* player = &scenario->players[playerId];

        // If the aid is not valid, remove the player
        if (aid == 0xFF) {
            player->playerType = RaceConfig::Player::TYPE_NONE;
            ctx->playerMiis.reset(playerId);
            ctx->playerRegions[playerId] = SystemManager::REGION_NONE;
            continue;
        }

        // Get the local player index by checking if the previous player's aid is identical
        u32 localPlayerId = 0;
        if (playerId > 0)
            localPlayerId = RKNetController::instance->aidPidMap.playerIds[playerId - 1] == aid;

        // Get the player info using the obtained data
        PlayerInfo* playerInfo = nullptr;
        for (int i = 0; i < playerCount; i++) {
            if (playerInfos[i].aid == aid && playerInfos[i].localPlayerIdx == localPlayerId) {
                playerInfo = &playerInfos[i];
                break;
            }
        }

        // If the player info is not found (should not happen), bail
        if (!playerInfo)
            continue;

        // Set the player type
        player->playerType = isLocal ? RaceConfig::Player::TYPE_LOCAL
                                     : RaceConfig::Player::TYPE_ONLINE;

        // Set the Mii
        if (isLocal) {
            ctx->playerMiis.copyFrom(&ctx->localPlayerMiis, localPlayerId, playerId);
        } else {
            ctx->playerMiis.copyFromManager(playerId, aid, localPlayerId);
        }

        // Get the per-player data from the SELECT packet
        RKNetSELECTPlayer* selectPlayer = selectHandler->getPlayerData(aid, localPlayerId);

        // Set character and vehicle (with failsaves)
        u8 character = selectPlayer->character;
        if (character >= CHARACTER_COUNT)
            character = MARIO;

        u8 vehicle = selectPlayer->vehicle;
        if (vehicle >= VEHICLE_COUNT)
            vehicle = STANDARD_BIKE_M;

        player->characterId = character;
        player->vehicleId = vehicle;

        // Set the previous position, score and star rank
        player->prevFinishPos = selectPlayer->prevRaceRank;
        player->previousScore = selectPlayer->sumPoints;
        ctx->setPlayerStarRank(playerId, selectPlayer->starRank);

        // Set the region id
        if (isLocal) {
            ctx->playerRegions[playerId] = SystemManager::instance->regionId;
        } else {
            ctx->playerRegions[playerId] = RKNetUSERHandler::instance->recvPackets[aid].region;
        }

        // Set VR/BR
        // For hosts, grab the correct VR/BR value based on the current menu
        if (localPlayerId == 0) {

            switch (curSection) {
                case Section::WIFI_VS_1P_VOTE:
                case Section::WIFI_VS_2P_VOTE:
                    player->rating.points = playerInfo->vr;
                    break;

                case Section::WIFI_BT_1P_VOTE:
                case Section::WIFI_BT_2P_VOTE:
                    player->rating.points = playerInfo->br;
                    break;
            }

        // For guests, default to 5000
        // TODO alternate point system for guests
        } else {
            player->rating.points = 5000;
        }

        // Set the team (if applicable)
        player->team = (settings->modeFlags & RaceConfig::Settings::FLAG_TEAMS)
                        ? playerInfo->team
                        : RaceConfig::Player::TEAM_NONE;
    }

    // Set up all the copied Miis
    ctx->copyPlayerMiis();
}
