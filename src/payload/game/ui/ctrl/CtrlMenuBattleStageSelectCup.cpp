#include "CtrlMenuBattleStageSelectCup.hpp"
#include <game/ui/ControlLoader.hpp>
#include <game/ui/UIUtils.hpp>
#include <mvp/cup/BattleCupSelectPageEx.hpp>
#include <mvp/cup/BattleStageSelectPageEx.hpp>
#include <mvp/cup/CupManager.hpp>
#include <platform/stdio.h>

///////////////////////
// Custom Cup System //
///////////////////////

// Update the loop size and set cup names and icons
REPLACE void CtrlMenuBattleStageSelectCup::initSelf() {

    // Get pages and selected cup
    BattleCupSelectPageEx* cupPage = BattleCupSelectPageEx::getPage();
    BattleStageSelectPageEx* coursePage = BattleStageSelectPageEx::getPage();
    const u32 selectedCup = cupPage->selectedButtonId;

    // Update each cup
    for (u32 i = 0; i < BattleCupSelectPageEx::getCupCount(); i++) {

        // Get cup
        CtrlMenuBattleStageSelectCupSub* cup = coursePage->getCupButton(i);

        // Set name
        const u16 cupIdx = CupManager::getCupIdxFromButton(i, cupPage->curPage, true);
        const u16 cupName = CupManager::GetCup(cupIdx, true)->cupName;
        cup->setText(cupName);

        // Set icon
        CupManager::updateCupButton(cup, cupPage->curPage, i, true);

        // Play switch animation
        cup->animator.getGroup(0)->setAnimation(0, 0.0f);
        cup->animator.getGroup(1)->setAnimation(i != selectedCup, 0.0f);
        cup->animator.getGroup(2)->setAnimation(i != selectedCup, 0.0f);
        cup->animator.getGroup(3)->setAnimation(i == selectedCup, 0.0f);

        // Call the virtual function
        cup->vf_3C(&elementPositions[POS_LAYOUT].trans);

        // Mark as selected if necessary
        cup->selected = (i == selectedCup);
    }
}

// Replace the BRCTR, update the loop size and set cup names and icons
REPLACE void CtrlMenuBattleStageSelectCup::load() {

    // Initialize main loader and get pages
    ControlLoader loader(this);
    BattleStageSelectPageEx* coursePage = BattleStageSelectPageEx::getPage();
    BattleCupSelectPageEx* cupPage = BattleCupSelectPageEx::getPage();

    // Load the main controller
    const u8 playerCount = UIUtils::getPlayerCount();
    const char* mainCtr = (playerCount <= 2) ? "CupSelectCupNULL" : "CupSelectCupNULL_4";
    loader.load("control", "CupSelectNULL", mainCtr, nullptr);

    // Initialize children
    initChildren(BattleCupSelectPageEx::getCupCount());
    for (u32 i = 0; i < BattleCupSelectPageEx::getCupCount(); i++) {

        // Get button variant
        char buffer[20];
        snprintf(buffer, sizeof(buffer), "Button%d", i);

        // Insert the button
        CtrlMenuBattleStageSelectCupSub* cup = coursePage->getCupButton(i);
        insertChild(i, cup);

        // Initialize it
        ControlLoader buttonLoader(cup);
        buttonLoader.load("control",
                          "CourseSelectCup",
                          buffer,
                          CtrlMenuBattleStageSelectCup::cupAnimNames);

        // Set cup name
        const u16 cupIdx = CupManager::getCupIdxFromButton(i, cupPage->curPage, true);
        const u16 cupName = CupManager::GetCup(cupIdx, true)->cupName;
        cup->setText(cupName);

        // Set cup icon
        CupManager::updateCupButton(cup, cupPage->curPage, i, true);
    }
}
