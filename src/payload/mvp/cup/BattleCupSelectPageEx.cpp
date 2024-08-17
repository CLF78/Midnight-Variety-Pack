#include "BattleCupSelectPageEx.hpp"
#include "CupManager.hpp"
#include <game/ui/Message.hpp>
#include <game/ui/UIUtils.hpp>

///////////////////////
// Custom Cup System //
///////////////////////

// Construct the expansion data
BattleCupSelectPageEx::BattleCupSelectPageEx() :
    onLeftHandler((BattleCupSelectArrow*)&arrows.leftButton, &BattleCupSelectArrow::onLeft),
    onRightHandler((BattleCupSelectArrow*)&arrows.rightButton, &BattleCupSelectArrow::onRight),
    curPage(CupManager::getCupPageFromTrack(SectionManager::instance->globalContext->lastStage, true)) {
        SET_HANDLER(arrows.onLeftHandler, onLeftHandler);
        SET_HANDLER(arrows.onRightHandler, onRightHandler);
        SET_HANDLER_FUNC(arrows.leftButton.onDeselectHandler, BattleCupSelectArrow::onDeselect);
        SET_HANDLER_FUNC(arrows.rightButton.onDeselectHandler, BattleCupSelectArrow::onDeselect);
        layoutCount++;
}

// Load the external layouts
UIControl* BattleCupSelectPageEx::loadLayout(u32 layoutIdx) {
    switch (layoutIdx) {

        case LYT_CUP_SELECT_CUP:
            insertChild(curChildCount++, &cupHolder, 0);
            cupHolder.load(1, false);
            return &cupHolder;

        case LYT_CUP_SELECT_COURSE:
            insertChild(curChildCount++, &stageHolder, 0);
            stageHolder.load();
            return &stageHolder;

        case LYT_CUP_ARROWS:
            insertChild(curChildCount++, &arrows, 0);

            // Determine the variant to use depending on the player count
            const char* rightVar = UIUtils::getPlayerCount() > 2 ? "ButtonArrowRight2" : "ButtonArrowRight";
            const char* leftVar = UIUtils::getPlayerCount() > 2 ? "ButtonArrowLeft2" : "ButtonArrowLeft";

            // Load BRCTR
            arrows.load("button", CUP_ARROW_R_BRCTR, rightVar, CUP_ARROW_L_BRCTR, leftVar, 1, false, false);
            return &arrows;

        // Invalid child
        default:
            return nullptr;
    }
}

// Set the starting cup button, skip the boundary check, adjust selection wrapping, disable the
// arrows if not required, update instruction text and display vote prompt if online
void BattleCupSelectPageEx::onActivate() {

    // Set the starting button
    const u32 lastStage = SectionManager::instance->globalContext->lastStage;
    selectedButtonId = CupManager::getCupButtonFromTrack(lastStage, curPage, true);

    // Do backend initialization
    MenuPage::onActivate();

    // Adjust X wrapping by setting the correct distance function
    // 0 wraps on the X and Y axis, 1 wraps on Y axis only
    const bool arrowsEnabled = CupManager::GetCupArrowsEnabled(true);
    const int wrapType = (CupManager::GetCupCount(true) == 2 || arrowsEnabled) ?
                    MultiControlInputManager::Y_WRAP :
                    MultiControlInputManager::XY_WRAP;
    multiControlInputManager.setDistanceFunc(wrapType);

    // Disable the arrows if not required
    arrows.configure(arrowsEnabled, arrowsEnabled);

    // Initialize cup holder
    cupHolder.init();

    // Set the instruction text according to the battle type
    const u32 battleType = RaceConfig::instance->menuScenario.settings.battleType;
    const u32 msgId = battleType == RaceConfig::Settings::BATTLE_BALLOON ?
                Message::Menu::INSTRUCTION_TEXT_BALLOON_BATTLE :
                Message::Menu::INSTRUCTION_TEXT_COIN_RUNNERS;
    instructionText->setText(msgId);

    // If we're offline, we're done
    if (!UIUtils::isOnlineRoom(SectionManager::instance->curSection->sectionId))
        return;

    // If we're entering the screen, add the Vote/Random prompt
    if (animId != Page::ANIM_NEXT)
        return;

    // Get the popup page
    YesNoPopupPageEx* popupPage = YesNoPopupPageEx::getPage();

    // Reset it and update the messages
    popupPage->reset();
    popupPage->setWindowMessage(Message::Menu::VOTE_FOR_STAGE_QUESTION);
    popupPage->configureButton(0, Message::Menu::VOTE, nullptr, Page::ANIM_NONE, nullptr);
    popupPage->configureButton(1, Message::Menu::VOTE_RANDOM, nullptr, Page::ANIM_NONE, nullptr);

    // Default to the Vote button
    popupPage->currSelected = 0;

    // Display the page and store a reference to it
    popupPage = (YesNoPopupPageEx*)addPage(Page::ONLINE_VOTE_PROMPT, Page::ANIM_NEXT);
    voteOrRandomPage = popupPage;
}

// Set the selected stage when a cup is clicked
void BattleCupSelectPageEx::setCourse(CtrlMenuBattleCupSelectCup* cupHolder, PushButton* button, u32 hudSlotId) {

    // Check for active state
    if (pageState == Page::STATE_ACTIVE) {

        // Update selected button
        selectedButtonId = cupHolder->currentSelected;

        // Get the cup and its first track
        const u16 cupIdx = CupManager::getCupIdxFromButton(selectedButtonId, curPage, true);
        const u16 trackIdx = CupManager::GetCup(cupIdx, true)->entryId[0];

        // Get the previous cup, and update the last selected stage if it differs
        const u16 prevCupIdx = CupManager::getCupIdxFromTrack(SectionManager::instance->globalContext->lastStage, true);
        if (cupIdx != prevCupIdx)
            SectionManager::instance->globalContext->lastStage = trackIdx;

        // Set the course data if offline
        if (!UIUtils::isOnlineRoom(SectionManager::instance->curSection->sectionId)) {

            // Get the actual track and store it
            const u16 actualTrackIdx = CupManager::getTrackFile(trackIdx);
            CupManager::SetCourse(&RaceConfig::instance->menuScenario.settings, actualTrackIdx);

        // Else wait for the course voting page to be loaded (is this even needed?)
        } else {
            while (SectionManager::instance->curSection->pages[Page::WIFI_VOTING] == nullptr) {}
        }

        // Go to the course select page
        loadNextPageById(Page::COURSE_SELECT_BT, button);
    }
}
