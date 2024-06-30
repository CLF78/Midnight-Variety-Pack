#include <common/Common.hpp>
#include <game/net/RKNetController.hpp>
#include <game/net/RKNetSelectHandler.hpp>
#include <game/ui/page/VotingBackPage.hpp>
#include <game/ui/page/WifiPlayerListPage.hpp>
#include <midnight/online/RepickQueue.hpp>

// Update the instruction text to be more accurate
REPLACE void WifiPlayerListPage::onActivate() {

    // Set up loop
    VotingBackPage* page = VotingBackPage::getPage();
    bool teamsEnabled = VotingBackPage::teamsEnabled();

    // Get rating type
    RatingType ratingType = RATING_NONE;
    switch (SectionManager::instance->curSection->sectionID) {

        case Section::WIFI_VS_1P_VOTE:
        case Section::WIFI_VS_2P_VOTE:
        case Section::WIFI_FROOM_1P_VS_VOTE:
        case Section::WIFI_FROOM_1P_TEAM_VS_VOTE:
        case Section::WIFI_FROOM_2P_VS_VOTE:
        case Section::WIFI_FROOM_2P_TEAM_VS_VOTE:
            ratingType = RATING_VS;
            break;

        case Section::WIFI_BT_1P_VOTE:
        case Section::WIFI_BT_2P_VOTE:
        case Section::WIFI_FROOM_1P_BALLOON_BATTLE_VOTE:
        case Section::WIFI_FROOM_1P_COIN_RUNNERS_VOTE:
        case Section::WIFI_FROOM_2P_BALLOON_BATTLE_VOTE:
        case Section::WIFI_FROOM_2P_COIN_RUNNERS_VOTE:
            ratingType = RATING_BT;
            break;
    }

    // Hide all player entries by default
    for (int i = 0; i < ARRAY_SIZE(playerEntries); i++) {
        playerEntries[i].hidden = true;
    }

    // If teams are enabled, distribute players based on the team
    if (teamsEnabled) {

        // Set up filling loop
        int redTeamIdx = 0;
        int blueTeamIdx = 1;
        int entryIdx;
    
        for (int playerIdx = 0; playerIdx < page->playerCount; playerIdx++) {

            // Get the team and the entry index
            RaceConfig::Player::Team playerTeam = (typeof(playerTeam))page->playerInfos[playerIdx].team;
            if (playerTeam == RaceConfig::Player::TEAM_BLUE) {
                entryIdx = blueTeamIdx;
                blueTeamIdx += 2;
            } else if (playerTeam == RaceConfig::Player::TEAM_RED) {
                entryIdx = redTeamIdx;
                redTeamIdx += 2;
            }

            // Set the entry
            setPlayerEntry(entryIdx, playerIdx, playerTeam, ratingType,
                           RKNetController::instance->isLocalPlayer(playerIdx));
        }

    // If teams are disabled, just set the players in order
    } else {
        for (int playerIdx = 0; playerIdx < page->playerCount; playerIdx++) {
            setPlayerEntry(playerIdx, playerIdx, RaceConfig::Player::TEAM_NONE,
                           ratingType, RKNetController::instance->isLocalPlayer(playerIdx));
        }
    }

    // Copy the repick queue
    LOG_DEBUG("Copying repick queue...");
    RepickQueue::instance.queue = RKNetSELECTHandler::instance->expansion.sendPacketEx.repickQueue;

    // Set the instruction text
    MessageInfo msgInfo;
    instructionText.setText(page->getInstructionText(&msgInfo), &msgInfo);
}
