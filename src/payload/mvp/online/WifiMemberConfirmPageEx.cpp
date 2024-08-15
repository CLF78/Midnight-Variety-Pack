#include "RepickQueue.hpp"
#include "WifiMemberConfirmPageEx.hpp"
#include <game/net/RKNetController.hpp>
#include <game/net/RKNetSelectHandler.hpp>
#include <game/ui/ControlLoader.hpp>
#include <game/ui/page/VotingBackPage.hpp>
#include <platform/stdio.h>

// Add the rule button to the page
void WifiMemberConfirmPageEx::onInit() {

    // Initialize input manager
    switch (SectionManager::instance->curSection->sectionId) {
        case Section::WIFI_VS_2P_VOTE:
        case Section::WIFI_BT_2P_VOTE:
        case Section::WIFI_FROOM_2P_VS_VOTE:
        case Section::WIFI_FROOM_2P_TEAM_VS_VOTE:
        case Section::WIFI_FROOM_2P_BALLOON_BATTLE_VOTE:
        case Section::WIFI_FROOM_2P_COIN_RUNNERS_VOTE:
            inputManager.init(1, true);
            break;

        default:
            inputManager.init(1, false);
            break;
    }

    // Start children initialization
    setInputManager(&inputManager);
    initChildren(ARRAY_SIZE(playerEntries) + 5);
    int currChildIdx = 0;

    // Add the title text
    insertChild(currChildIdx++, &titleText, 0);
    titleText.load(false);

    // Set the message
    switch (SectionManager::instance->curSection->sectionId) {
        case Section::WIFI_VS_1P_VOTE:
        case Section::WIFI_VS_2P_VOTE:
        case Section::WIFI_FROOM_1P_VS_VOTE:
        case Section::WIFI_FROOM_2P_VS_VOTE:
            titleText.setText(Message::Menu::TITLE_TEXT_VR_SCREEN);
            break;

        case Section::WIFI_BT_1P_VOTE:
        case Section::WIFI_BT_2P_VOTE:
        case Section::WIFI_FROOM_1P_TEAM_VS_VOTE:
        case Section::WIFI_FROOM_2P_TEAM_VS_VOTE:
        case Section::WIFI_FROOM_1P_BALLOON_BATTLE_VOTE:
        case Section::WIFI_FROOM_2P_BALLOON_BATTLE_VOTE:
        case Section::WIFI_FROOM_1P_COIN_RUNNERS_VOTE:
        case Section::WIFI_FROOM_2P_COIN_RUNNERS_VOTE:
            titleText.setText(Message::Menu::TITLE_TEXT_VR_SCREEN_TEAMS);
            break;
    }

    // Add the instruction text
    insertChild(currChildIdx++, &instructionText, 0);
    instructionText.load();

    // Add the OK button
    insertChild(currChildIdx++, &okButton, 0);
    okButton.load("button", "WifiMemberConfirmButton", "ButtonOK", 1, false, false);
    okButton.setOnClickHandler(&onButtonClickHandler, 0);

    // Add the change combo button
    // TODO add handler
    insertChild(currChildIdx++, &changeComboButton, 0);
    changeComboButton.load("button", "WifiMemberConfirmButton", "ButtonChangeChar", 1, false, false);

    // Add the rule button
    insertChild(currChildIdx++, &ruleButton, 0);
    ruleButton.load("button", "WifiMemberConfirmButton", "ButtonRules", 1, false, false);
    ruleButton.setOnClickHandler(&onRuleButtonClickHandler, 0);

    // Add the player entries
    for (u32 i = 0; i < ARRAY_SIZE(playerEntries); i++) {

        // Insert the entry
        LayoutUIControl* entry = &playerEntries[i];
        insertChild(currChildIdx++, entry, 0);

        // Load the BRCTR
        ControlLoader loader(entry);
        char buffer[16];
        snprintf(buffer, sizeof(buffer), "Member%02d", i);
        loader.load("control", "WifiMemberConfirm", buffer, WifiMemberConfirmPage::animNames);

        // Something with animations
        entry->animator.getGroup(0)->setAnimation(0, 0.0f);
        entry->animator.getGroup(1)->setAnimation(1, 0.0f);
        entry->animator.getGroup(2)->setAnimation(1, 0.0f);
    }

    // Set the OK button as default selection
    okButton.selectDefault(0);
}

// Update the instruction text to be more accurate
void WifiMemberConfirmPageEx::onActivate() {

    // Set up loop
    VotingBackPage* page = VotingBackPage::getPage();
    const bool teamsEnabled = VotingBackPage::teamsEnabled();

    // Get rating type
    RatingType ratingType = RATING_NONE;
    switch (SectionManager::instance->curSection->sectionId) {

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
    for (u32 i = 0; i < ARRAY_SIZE(playerEntries); i++) {
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
            const RaceConfig::Player::Team playerTeam = page->playerInfos[playerIdx].team;
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

// Ensure the rules popup is closed when the timer expires
void WifiMemberConfirmPageEx::afterCalc() {

    // This check is necessary to ensure the procedure is not repeated twice
    if (pageState != Page::STATE_ACTIVE && rulePopup == nullptr)
        return;

    if (timer == nullptr)
        return;

    if (timer->value > 0.0f)
        return;

    // Ensure the pointer is nulled to prevent running this twice
    if (rulePopup) {
        rulePopup->handleFront();
        rulePopup = nullptr;
    }

    replace(ANIM_NEXT, 0.0f);
}

// Display the rule popup when the rule button is pressed
void WifiMemberConfirmPageEx::onRuleButtonClick(PushButton* button, u32 playerFlags) {
    MessagePopupPage* popupPage = MessagePopupPage::getPage();
    popupPage->reset();
    popupPage->setWindowMessage(3309);
    popupPage->playerFlags = 1;
    addPage(Page::MESSAGE_POPUP, 0);
    rulePopup = popupPage;
}
