#include <common/Common.hpp>
#include <game/ui/ControlLoader.hpp>
#include <game/ui/UIUtils.hpp>
#include <game/ui/ctrl/CtrlMenuBattleStageSelectCup.hpp>
#include <midnight/cup/BattleCupSelectPageEx.hpp>
#include <midnight/cup/BattleStageSelectPageEx.hpp>
#include <midnight/cup/CupManager.hpp>
#include <platform/stdio.h>

///////////////////////
// Custom Cup System //
///////////////////////

// Update the loop size and set cup names and icons
REPLACE void CtrlMenuBattleStageSelectCup::initSelf() {

    // Get pages and selected cup
    BattleCupSelectPageEx* cupPage = BattleCupSelectPageEx::getPage();
    BattleStageSelectPageEx* coursePage = BattleStageSelectPageEx::getPage();
    u32 selectedCup = cupPage->selectedButtonId;

    // Update each cup
    for (int i = 0; i < BattleCupSelectPageEx::getCupCount(); i++) {

        // Get cup
        CtrlMenuBattleStageSelectCupSub* cup = coursePage->getCupButton(i);

        // Set name
        u32 cupIdx = CupManager::getCupIdxFromButton(i, cupPage->curPage, true);
        u16 cupName = CupManager::GetCupList(true)[cupIdx].cupName;
        cup->setText(cupName, nullptr);

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
    u8 playerCount = UIUtils::getPlayerCount();
    const char* mainCtr = (playerCount <= 2) ? "CupSelectCupNULL" : "CupSelectCupNULL_4";
    loader.load("control", "CupSelectNULL", mainCtr, nullptr);

    // Initialize children
    initChildren(BattleCupSelectPageEx::getCupCount());
    for (int i = 0; i < BattleCupSelectPageEx::getCupCount(); i++) {

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
        u32 cupIdx = CupManager::getCupIdxFromButton(i, cupPage->curPage, true);
        u16 cupName = CupManager::GetCupList(true)[cupIdx].cupName;
        cup->setText(cupName, nullptr);

        // Set cup icon
        CupManager::updateCupButton(cup, cupPage->curPage, i, true);
    }
}
