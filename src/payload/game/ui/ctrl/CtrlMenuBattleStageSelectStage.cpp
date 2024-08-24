#include "CtrlMenuBattleStageSelectStage.hpp"
#include <game/ui/ControlLoader.hpp>
#include <mvp/cup/BattleCupSelectPageEx.hpp>
#include <mvp/cup/BattleStageSelectPageEx.hpp>
#include <mvp/cup/CupManager.hpp>
#include <platform/stdio.h>

///////////////////////
// Custom Cup System //
///////////////////////

// Update arena names
REPLACE void CtrlMenuBattleStageSelectStage::initSelf() {

    BattleCupSelectPageEx* cupPage = BattleCupSelectPageEx::getPage();
    BattleStageSelectPageEx* coursePage = BattleStageSelectPageEx::getPage();
    const u32 selectedCup = cupPage->selectedButtonId;
    const u32 lastStage = SectionManager::instance->globalContext->lastStage;
    int selected = -1;

    for (u32 i = 0; i < ARRAY_SIZE(courseButtons); i++) {

        // Get button
        PushButton* trackButton = &courseButtons[i];

        // Set name
        const u16 cupIdx = CupManager::getCupIdxFromButton(selectedCup, cupPage->curPage, true);
        const u16 trackIdx = CupManager::GetCup(cupIdx, true)->entryId[i];
        CupManager::setTrackName(trackButton, trackIdx);

        // Set button id
        trackButton->buttonId = (int)i;

        // Play animation
        trackButton->animator.getGroup(4)->setAnimation(0, 0.0f);

        // Select the button if the track matches
        if (lastStage == trackIdx) {
            coursePage->setSelection(trackButton);
            selected = (int)i;
        }
    }

    if (selected == -1) {
        coursePage->setSelection(&courseButtons[0]);
    }
}

// Replace the BRCTR and update the child count
REPLACE void CtrlMenuBattleStageSelectStage::load(u32 playerFlags, bool unk) {

    // Initialize main loader
    ControlLoader loader(this);

    // Load the main controller
    const u8 playerCount = UIUtils::getPlayerCount();
    const char* mainCtr = (playerCount <= 2) ? "CupSelectCourseNULL" : "CupSelectCourseNULL_4";
    loader.load("control", "CupSelectNULL", mainCtr, nullptr);

    // Initialize children
    initChildren(ARRAY_SIZE(courseButtons));
    for (u32 i = 0; i < ARRAY_SIZE(courseButtons); i++) {

        // Get button and its variant
        CtrlMenuMovieButton* button = &courseButtons[i];
        char buffer[20];
        snprintf(buffer, sizeof(buffer), "Button%d", i);

        // Insert it
        insertChild(i, button);

        // Initialize it
        button->loadWithAnims(buttonAnims, "button", "BattleStageSelectStage", buffer, playerFlags, unk);
        button->setOnClickHandler(&onButtonClickHandler, 0);
        button->setOnSelectHandler(&onButtonSelectHandler);
    }

    // Set first button as default selection
    courseButtons[0].selectDefault(0);
}

// Set the selected course
REPLACE void CtrlMenuBattleStageSelectStage::onButtonClick(PushButton* btn, u32 hudSlotId) {
    BattleStageSelectPageEx::getPage()->setCourse(this, btn, hudSlotId);
}

// Disable button movies
REPLACE void CtrlMenuBattleStageSelectStage::onButtonSelect(PushButton* btn, u32 hudSlotId) {}
