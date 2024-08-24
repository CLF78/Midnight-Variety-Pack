#include "BattleStageSelectPageEx.hpp"
#include "BattleCupSelectPageEx.hpp"
#include "CupManager.hpp"
#include <game/sound/SoundEffect.hpp>
#include <game/ui/Message.hpp>
#include <game/ui/UIUtils.hpp>
#include <game/ui/page/VotingPage.hpp>
#include <mvp/online/RepickQueue.hpp>

///////////////////////
// Custom Cup System //
///////////////////////

// Add the additional cup buttons and replace some handlers
BattleStageSelectPageEx::BattleStageSelectPageEx() :
    onRepickPromptPressHandler(this, &BattleStageSelectPageEx::onRepickPromptPress) {
    SET_HANDLER_FUNC(onButtonClickHandler, BattleStageSelectPageEx::onButtonClick);
    SET_HANDLER_FUNC(onBackPressHandler, BattleStageSelectPageEx::onBackPress);
}

void BattleStageSelectPageEx::onRepickPromptPress(s32 choice, PushButton* button) {

    // If Vote Anyway is selected, go to the next page
    if (choice == 0) {
        nextPageId = Page::NONE;
        replace(Page::ANIM_NEXT, button->getDelay());
    }

    // Remove the prompt reference anyway
    repickPrompt = nullptr;
}

// Properly store/vote the selected arena
void BattleStageSelectPageEx::setCourse(CtrlMenuBattleStageSelectStage* courseHolder, PushButton* button,
                                        u32 hudSlotId) {
    // Check for active state
    if (pageState != Page::STATE_ACTIVE) {
        return;
    }

    // Get selected arena and set it as last stage
    BattleCupSelectPageEx* cupPage = BattleCupSelectPageEx::getPage();
    const u16 cupIdx = CupManager::getCupIdxFromButton(cupPage->selectedButtonId, cupPage->curPage, true);
    const u16 trackIdx = CupManager::GetCup(cupIdx, true)->entryId[button->buttonId];
    SectionManager::instance->globalContext->lastStage = trackIdx;

    if (UIUtils::isOnlineRoom(SectionManager::instance->curSection->sectionId)) {

        // Submit the selected arena
        VotingPage* votingPage = VotingPage::getPage();
        votingPage->submitVote(trackIdx);

        // If the track is in the repick queue, show a popup
        if (RepickQueue::instance.GetQueuePosition(trackIdx) != RepickQueue::NOT_IN_QUEUE) {

            // Get the popup
            YesNoPopupPageEx* popupPage = YesNoPopupPageEx::getPage();

            // Get handler
            InputHandler2<Page, void, s32, PushButton*>* repickHandler;
            repickHandler = SET_HANDLER(repickHandler, onRepickPromptPressHandler);

            // Reset the popup and update the messages
            popupPage->reset();
            popupPage->setWindowMessage(Message::Menu::TRACK_UNPICKABLE_PROMPT_BT);
            popupPage->configureButton(0, Message::Menu::TRACK_UNPICKABLE_VOTE_ANYWAY, nullptr,
                                       Page::ANIM_NONE, repickHandler);
            popupPage->configureButton(1, Message::Menu::TRACK_UNPICKABLE_GO_BACK, nullptr, Page::ANIM_NONE,
                                       repickHandler);

            // Default to the Go Back button and allow going back
            popupPage->currSelected = 1;
            popupPage->isBackButtonEnabled = true;
            popupPage->onBackSelectedButton = 1;

            // Display the page
            repickPrompt = (YesNoPopupPageEx*)addPage(Page::ONLINE_VOTE_PROMPT, Page::ANIM_NEXT);
        }

        // Else go to the next page
        else {
            nextPageId = Page::NONE;
            replace(Page::ANIM_NEXT, button->getDelay());
        }

        // Report stage as selected
        stageSelected = true;
        return;
    }

    // Get the actual arena to be played and store it
    const u16 actualTrackIdx = CupManager::getTrackFile(trackIdx);
    CupManager::SetCourse(&RaceConfig::instance->menuScenario.settings, actualTrackIdx);

    // Prepare intro
    requestSectionChange(Section::DEMO_BT, button);

    // Generate arena order from the selected one
    CupManager::generateCourseOrder(cupIdx, button->buttonId, true);

    // Report stage as selected
    stageSelected = true;
}

// Disable movies and simplify instruction text setting
void BattleStageSelectPageEx::onActivate() {

    // Set the instruction text according to the battle type
    const u32 battleType = RaceConfig::instance->menuScenario.settings.battleType;
    const u32 msgId = (battleType == RaceConfig::Settings::BATTLE_BALLOON) ?
                          Message::Menu::INSTRUCTION_TEXT_BALLOON_BATTLE :
                          Message::Menu::INSTRUCTION_TEXT_COIN_RUNNERS;
    instructionText->setText(msgId);

    // Do base activation
    MenuPage::onActivate();
    multiControlInputManager.setDistanceFunc(MultiControlInputManager::Y_WRAP);
}

// Force press the selected option on the repick prompt when the timer runs out
void BattleStageSelectPageEx::afterCalc() {

    // Check for active state
    if (pageState != Page::STATE_ACTIVE) {
        return;
    }

    // Check if we are online
    if (!UIUtils::isOnlineRoom(SectionManager::instance->curSection->sectionId)) {
        return;
    }

    // Check if the timer is zero
    if (timer->value > 0.0f) {
        return;
    }

    // If the prompt is not enabled, vote random
    if (repickPrompt == nullptr) {
        forceRandomVote();
    }

    // Else force press the selected button
    else {
        repickPrompt->forcePressSelected();
    }
}

// Apply properties to the extra cups when pressing the on-screen back button
void BattleStageSelectPageEx::onButtonClick(PushButton* button, u32 hudSlotId) {

    // Check for active state
    if (pageState != Page::STATE_ACTIVE) {
        return;
    }

    // Skip if the button isn't the back button
    if (button->buttonId != MenuPage::BACK_BUTTON) {
        return;
    }

    // Apply properties
    for (u32 i = 0; i < getCupCount(); i++) {
        CtrlMenuBattleStageSelectCupSub* cupButton = getCupButton(i);
        cupButton->alpha = 300.0f;
        cupButton->fadeDirection = 1;
    }

    // Play back button sound
    playSound(SE_UI_PAGE_PREV, -1);
}

// Apply properties to the extra cups when pressing the back button
void BattleStageSelectPageEx::onBackPress(u32 hudSlotId) {

    // Check for active state
    if (pageState != Page::STATE_ACTIVE) {
        return;
    }

    // Apply properties
    for (u32 i = 0; i < getCupCount(); i++) {
        CtrlMenuBattleStageSelectCupSub* cupButton = getCupButton(i);
        cupButton->alpha = 300.0f;
        cupButton->fadeDirection = 1;
    }

    // Play back button sound
    playSound(SE_UI_PAGE_PREV, -1);
}
