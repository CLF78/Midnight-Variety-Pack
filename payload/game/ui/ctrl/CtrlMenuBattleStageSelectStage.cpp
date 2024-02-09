#include <common/Common.hpp>
#include <game/ui/ctrl/CtrlMenuBattleStageSelectStage.hpp>
#include <game/ui/page/BattleStageSelectPage.hpp>
#include <game/ui/page/BattleCupSelectPage.hpp>
#include <game/ui/ControlLoader.hpp>
#include <game/ui/SectionManager.hpp>
#include <midnight/cup/CupManager.hpp>
#include <platform/stdio.h>

///////////////////////
// Custom Cup System //
///////////////////////

// CtrlMenuBattleStageSelectStage::initSelf() override
// Update arena names
kmPointerDefCpp(0x808D2F18, void, CtrlMenuBattleStageSelectStage* self) {

    BattleCupSelectPage* cupPage = BattleCupSelectPage::getPage();
    BattleStageSelectPage* coursePage = BattleStageSelectPage::getPage();
    u32 selectedCup = cupPage->selectedButtonId;
    u32 lastStage = SectionManager::instance->globalContext->lastStage;
    int selected = -1;

    for (int i = 0; i < 4; i++) {

        // Get button
        PushButton* trackButton = &self->courseButtons[i];

        // Set name
        u32 cupIdx = CupManager::getCupIdxFromButton(selectedCup, cupPage->extension.curPage, true);
        u32 trackIdx = CupManager::GetCupList(true)[cupIdx].entryId[i];
        trackButton->setText(CupManager::getTrackName(trackIdx), nullptr);

        // Set button id
        trackButton->buttonId = i;

        // Play animation
        trackButton->animator.getGroup(4)->setAnimation(0, 0.0f);

        // Select the button if the track matches
        if (lastStage == trackIdx) {
            coursePage->setSelection(trackButton);
            selected = i;
        }
    }

    if (selected == -1)
        coursePage->setSelection((PushButton*)&self->courseButtons[0]);
}

// CtrlMenuBattleStageSelectStage::load() override
// Replace the BRCTR and update the child count
kmBranchDefCpp(0x807E1D80, NULL, void, CtrlMenuBattleStageSelectStage* self, u32 playerFlags, bool unk) {

    // Initialize main loader
    ControlLoader loader(self);

    // Load the main controller
    u8 playerCount = UIUtils::getPlayerCount();
    const char* mainCtr = (playerCount <= 2) ? "CupSelectCourseNULL" : "CupSelectCourseNULL_4";
    loader.load("control", "CupSelectNULL", mainCtr, nullptr);

    // Initialize children
    self->initChildren(4);
    for (int i = 0; i < 4; i++) {

        // Get button and its variant
        CtrlMenuMovieButton* button = &self->courseButtons[i];
        char buffer[20];
        snprintf(buffer, sizeof(buffer), "Button%d", i);

        // Insert it
        self->insertChild(i, button);

        // Initialize it
        button->loadWithAnims(CtrlMenuBattleStageSelectStage::buttonAnims, "button",
                              "BattleStageSelectStage", buffer, playerFlags, unk);
        button->setOnClickHandler(&self->onClickHandler, 0);
        button->setOnSelectHandler(&self->onSelectHandler);
    }

    // Set first button as default selection
    self->courseButtons[0].selectDefault(0);
}

// CtrlMenuBattleStageSelectStage::onSelect() override
// Disable button movies
kmPointerDefCpp(0x808BC440, void, CtrlMenuBattleStageSelectStage* self, PushButton* button, u32 unk) {}
