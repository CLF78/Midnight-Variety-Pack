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

// Update arena names
REPLACE void CtrlMenuBattleStageSelectStage::initSelf() {

    BattleCupSelectPage* cupPage = BattleCupSelectPage::getPage();
    BattleStageSelectPage* coursePage = BattleStageSelectPage::getPage();
    u32 selectedCup = cupPage->selectedButtonId;
    u32 lastStage = SectionManager::instance->globalContext->lastStage;
    int selected = -1;

    for (int i = 0; i < ARRAY_SIZE(courseButtons); i++) {

        // Get button
        PushButton* trackButton = &courseButtons[i];

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
        coursePage->setSelection(&courseButtons[0]);
}

// Replace the BRCTR and update the child count
REPLACE void CtrlMenuBattleStageSelectStage::load(u32 playerFlags, bool unk) {

    // Initialize main loader
    ControlLoader loader(this);

    // Load the main controller
    u8 playerCount = UIUtils::getPlayerCount();
    const char* mainCtr = (playerCount <= 2) ? "CupSelectCourseNULL" : "CupSelectCourseNULL_4";
    loader.load("control", "CupSelectNULL", mainCtr, nullptr);

    // Initialize children
    initChildren(ARRAY_SIZE(courseButtons));
    for (int i = 0; i < ARRAY_SIZE(courseButtons); i++) {

        // Get button and its variant
        CtrlMenuMovieButton* button = &courseButtons[i];
        char buffer[20];
        snprintf(buffer, sizeof(buffer), "Button%d", i);

        // Insert it
        insertChild(i, button);

        // Initialize it
        button->loadWithAnims(CtrlMenuBattleStageSelectStage::buttonAnims, "button",
                              "BattleStageSelectStage", buffer, playerFlags, unk);
        button->setOnClickHandler(&onClickHandler, 0);
        button->setOnSelectHandler(&onSelectHandler);
    }

    // Set first button as default selection
    courseButtons[0].selectDefault(0);
}

// CtrlMenuBattleStageSelectStage::onSelect() override
// Disable button movies
REPLACE void CtrlMenuBattleStageSelectStage::onCupSelect(PushButton* btn, u32 unk) {}
