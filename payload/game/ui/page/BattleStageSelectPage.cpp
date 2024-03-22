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

// Section::createPage() patch (here for convenience)
// Update memory size of page
kmCallDefCpp(0x80623E9C, u32) {
    return sizeof(BattleStageSelectPage);
}

// BattleStageSelectPage::BattleStageSelectPage() patch
// Construct the expansion data
kmBranchDefCpp(0x80629CE4, NULL, BattleStageSelectPage*, BattleStageSelectPage* self) {

    // Construct the cup buttons
    for (int i = 0; i < ARRAY_SIZE(self->extension.cups); i++)
        CtrlMenuBattleStageSelectCupSub::construct(&self->extension.cups[i]);

    return self;
}

// BattleStageSelectPage::onActivate() patch
// Turn off background movies
kmWrite32(0x8083CD94, 0x60000000);

// BattleStageSelectPage::~BattleStageSelectPage() patch
// Destroy the expansion data
kmCallDefCpp(0x8083D360, void, BattleStageSelectPage* self) {

    // Delete the cup buttons
    for (int i = 0; i < ARRAY_SIZE(self->extension.cups); i++)
        self->extension.cups[i].~CtrlMenuBattleStageSelectCupSub();
}

// BattleStageSelectPage::setCourse() override
// Properly store/vote the selected arena
kmBranchDefCpp(0x8083CFE8, NULL, void, BattleStageSelectPage* self, CtrlMenuBattleStageSelectStage* courseHolder, PushButton* button) {

    // Check if the page is being defocused
    if (self->pageState != Page::STATE_DEFOCUSING)
        return;

    // Get selected arena and set it as last stage
    BattleCupSelectPage* cupPage = BattleCupSelectPage::getPage();
    u32 cupIdx = CupManager::getCupIdxFromButton(cupPage->selectedButtonId, cupPage->extension.curPage, true);
    u32 trackIdx = CupManager::GetCupList(true)[cupIdx].entryId[button->buttonId];
    SectionManager::instance->globalContext->lastStage = trackIdx;

    if (UIUtils::isOnlineRoom(SectionManager::instance->curSection->sectionID)) {

        // Submit the selected arena
        VotingPage* votingPage = VotingPage::getPage();
        votingPage->submitVote(trackIdx);

        // Go to the next page
        self->nextPageId = Page::NONE;
        self->replace(Page::ANIM_NEXT, button->getDelay());

    } else {

        // Get the actual arena to be played and store it
        u32 actualTrackIdx = CupManager::getTrackFile(trackIdx);
        CupManager::SetCourse(&RaceConfig::instance->menuScenario.settings, actualTrackIdx);

        // Prepare intro
        self->requestSectionChange(Section::DEMO_BT, button);

        // Generate arena order from the selected one
        CupManager::generateCourseOrder(cupIdx, button->buttonId, true);
    }

    // Report stage as selected
    self->stageSelected = true;
}

// BattleStageSelectPage::onBtnClick() override
// Apply properties to the extra cups when pressing the on-screen back button
kmPointerDefCpp(0x808BC3F8, void, BattleStageSelectPage* self, PushButton* button) {

    // Skip if not defocusing
    if (self->pageState != Page::STATE_DEFOCUSING)
        return;

    // Skip if the button isn't the back button
    if (button->buttonId != MenuPage::BACK_BUTTON)
        return;

    // Apply properties
    for (int i = 0; i < 8; i++) {
        CtrlMenuBattleStageSelectCupSub* cupButton = self->getCupButton(i);
        cupButton->alpha = 300.0f;
        cupButton->fadeDirection = 1;
    }

    // Play back button sound
    self->playSound(SE_UI_PAGE_PREV, -1);
}

// BattleStageSelectPage::onBackPress() override
// Apply properties to the extra cups when pressing the back button
kmPointerDefCpp(0x808BC41C, void, BattleStageSelectPage* self) {

    // Skip if not defocusing
    if (self->pageState != Page::STATE_DEFOCUSING)
        return;

    // Apply properties
    for (int i = 0; i < 8; i++) {
        CtrlMenuBattleStageSelectCupSub* cupButton = self->getCupButton(i);
        cupButton->alpha = 300.0f;
        cupButton->fadeDirection = 1;
    }

    // Play back button sound
    self->playSound(SE_UI_PAGE_PREV, -1);
}
