#include <common/Common.hpp>
#include <game/ui/page/BattleCupSelectPage.hpp>
#include <game/ui/page/BattleStageSelectPage.hpp>
#include <game/ui/ctrl/CtrlMenuBattleStageSelectCup.hpp>
#include <game/ui/ControlLoader.hpp>
#include <game/ui/UIUtils.hpp>
#include <midnight/cup/CupManager.hpp>
#include <platform/stdio.h>

///////////////////////////////////
// Patches for Custom Cup System //
///////////////////////////////////

// CtrlMenuBattleStageSelectCup::load() override
// Replace the BRCTR, update the loop size and set cup names and icons
kmCallDefCpp(0x8083CCF8, void, CtrlMenuBattleStageSelectCup* self) {

    // Initialize main loader and get pages
    ControlLoader loader(self);
    BattleStageSelectPage* coursePage = BattleStageSelectPage::getPage();
    BattleCupSelectPage* cupPage = BattleCupSelectPage::getPage();

    // Load the main controller
    u8 playerCount = UIUtils::getPlayerCount();
    const char* mainCtr = (playerCount <= 2) ? "CupSelectCupNULL" : "CupSelectCupNULL_4";
    loader.load("control", "BattleCupSelectNULL", mainCtr, nullptr);

    // Initialize children
    self->initChildren(8);
    for (int i = 0; i < 8; i++) {

        // Get button variant
        char buffer[20];
        snprintf(buffer, sizeof(buffer), "Button%d", i);

        // Insert the button
        CtrlMenuBattleStageSelectCupSub* cup = coursePage->getCupButton(i);
        self->insertChild(i, cup);

        // Initialize it
        ControlLoader buttonLoader(cup);
        buttonLoader.load("control",
                          "CourseSelectCup",
                          buffer,
                          CtrlMenuBattleStageSelectCup::cupAnimNames);

        // Set cup name
        u32 cupIdx = CupManager::getCupIdxFromButton(i, cupPage->extension.curPage, true);
        u16 cupName = CupManager::GetCupList(true)[cupIdx].cupName;
        cup->setText(cupName, nullptr);

        // Set cup icon
        CupManager::updateCupButton(cup, cupPage->extension.curPage, i, true);
    }
}

// CtrlMenuBattleStageSelectCup::onInit() override
// Update the loop size and set cup names and icons
kmPointerDefCpp(0x808D2FD0, void, CtrlMenuBattleCupSelectStage* self) {

    // Get pages and selected cup
    BattleCupSelectPage* cupPage = BattleCupSelectPage::getPage();
    BattleStageSelectPage* coursePage = BattleStageSelectPage::getPage();
    u32 selectedCup = cupPage->selectedButtonId;

    // Update each cup
    for (int i = 0; i < 8; i++) {

        // Get cup
        CtrlMenuBattleStageSelectCupSub* cup = coursePage->getCupButton(i);
        
        // Set name
        u32 cupIdx = CupManager::getCupIdxFromButton(i, cupPage->extension.curPage, true);
        u16 cupName = CupManager::GetCupList(true)[cupIdx].cupName;
        cup->setText(cupName, nullptr);

        // Set icon
        CupManager::updateCupButton(cup, cupPage->extension.curPage, i, true);

        // Play switch animation
        cup->animator.getGroup(0)->setAnimation(0, 0.0f);
        cup->animator.getGroup(1)->setAnimation(i != selectedCup, 0.0f);
        cup->animator.getGroup(2)->setAnimation(i != selectedCup, 0.0f);
        cup->animator.getGroup(3)->setAnimation(i == selectedCup, 0.0f);

        // Call the virtual function
        cup->vf_3C(&self->elementPositions[POS_LAYOUT].trans);

        // Mark as selected if necessary
        cup->selected = (i == selectedCup);
    }
}
