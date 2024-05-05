#include <common/Common.hpp>
#include <game/sound/SoundEffect.hpp>
#include <game/ui/page/BattleStageSelectPage.hpp>
#include <game/ui/page/BattleCupSelectPage.hpp>
#include <game/ui/page/VotingPage.hpp>
#include <game/ui/SectionManager.hpp>
#include <game/ui/UIUtils.hpp>
#include <midnight/cup/CupManager.hpp>

///////////////////////
// Custom Cup System //
///////////////////////

// Add the additional cup buttons and replace some handlers
BattleStageSelectPageEx::BattleStageSelectPageEx() : cups() {
    onBtnClick.handle = (typeof(onBtnClick.handle))&handleBtnClick;
    onBackPress.handle = (typeof(onBackPress.handle))&handleBackPress;
}

// Properly store/vote the selected arena
void BattleStageSelectPageEx::setCourse(CtrlMenuBattleStageSelectStage* courseHolder, PushButton* button, int unk) {

    // Check if the page is being defocused
    if (pageState != Page::STATE_DEFOCUSING)
        return;

    // Get selected arena and set it as last stage
    BattleCupSelectPageEx* cupPage = BattleCupSelectPageEx::getPage();
    u32 cupIdx = CupManager::getCupIdxFromButton(cupPage->selectedButtonId, cupPage->curPage, true);
    u32 trackIdx = CupManager::GetCupList(true)[cupIdx].entryId[button->buttonId];
    SectionManager::instance->globalContext->lastStage = trackIdx;

    if (UIUtils::isOnlineRoom(SectionManager::instance->curSection->sectionID)) {

        // Submit the selected arena
        VotingPage* votingPage = VotingPage::getPage();
        votingPage->submitVote(trackIdx);

        // Go to the next page
        nextPageId = Page::NONE;
        replace(Page::ANIM_NEXT, button->getDelay());

    } else {

        // Get the actual arena to be played and store it
        u32 actualTrackIdx = CupManager::getTrackFile(trackIdx);
        CupManager::SetCourse(&RaceConfig::instance->menuScenario.settings, actualTrackIdx);

        // Prepare intro
        requestSectionChange(Section::DEMO_BT, button);

        // Generate arena order from the selected one
        CupManager::generateCourseOrder(cupIdx, button->buttonId, true);
    }

    // Report stage as selected
    stageSelected = true;
}

// Apply properties to the extra cups when pressing the on-screen back button
void BattleStageSelectPageEx::handleBtnClick(PushButton* button) {

    // Skip if not defocusing
    if (pageState != Page::STATE_DEFOCUSING)
        return;

    // Skip if the button isn't the back button
    if (button->buttonId != MenuPage::BACK_BUTTON)
        return;

    // Apply properties
    for (int i = 0; i < getCupCount(); i++) {
        CtrlMenuBattleStageSelectCupSub* cupButton = getCupButton(i);
        cupButton->alpha = 300.0f;
        cupButton->fadeDirection = 1;
    }

    // Play back button sound
    playSound(SE_UI_PAGE_PREV, -1);
}

// Apply properties to the extra cups when pressing the back button
void BattleStageSelectPageEx::handleBackPress(int playerId) {

    // Skip if not defocusing
    if (pageState != Page::STATE_DEFOCUSING)
        return;

    // Apply properties
    for (int i = 0; i < getCupCount(); i++) {
        CtrlMenuBattleStageSelectCupSub* cupButton = getCupButton(i);
        cupButton->alpha = 300.0f;
        cupButton->fadeDirection = 1;
    }

    // Play back button sound
    playSound(SE_UI_PAGE_PREV, -1);
}

// BattleStageSelectPage::onActivate() patch
// Turn off background movies
kmWrite32(0x8083CD94, 0x60000000);
