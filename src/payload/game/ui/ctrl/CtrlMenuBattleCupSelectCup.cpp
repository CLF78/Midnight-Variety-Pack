#include <game/ui/ControlLoader.hpp>
#include <game/ui/UIUtils.hpp>
#include <mvp/cup/BattleCupSelectPageEx.hpp>
#include <mvp/cup/CupManager.hpp>
#include <platform/stdio.h>

///////////////////////
// Custom Cup System //
///////////////////////

// Remove cup boundary check and movie-related code
REPLACE void CtrlMenuBattleCupSelectCup::initSelf() {

    // Get page
    BattleCupSelectPageEx* page = BattleCupSelectPageEx::getPage();

    // Replace loop with a straight function
    // This does not have any safeguards and assumes the selectedButtonId value is valid
    PushButton* cupButton = page->getCupButton(page->selectedButtonId);
    page->setSelection(cupButton); // is this needed?
    currentSelected = cupButton->buttonId;
    page->setCourseNames(this, cupButton, 0);
}

// Replace the BRCTR, update the button loop size, set cup names and icons and remove the background movies
REPLACE void CtrlMenuBattleCupSelectCup::load(u32 playerFlags, bool unk) {

    // Initialize loader and get page
    ControlLoader loader(this);
    BattleCupSelectPageEx* page = BattleCupSelectPageEx::getPage();

    // Set starting button to the first one
    currentSelected = 0;

    // Load the main controller
    const u8 playerCount = UIUtils::getPlayerCount();
    const char* mainCtr = (playerCount <= 2) ? "CupSelectCupNULL" : "CupSelectCupNULL_4";
    loader.load("control", "CupSelectNULL", mainCtr, nullptr);

    // Initialize children
    initChildren(BattleCupSelectPageEx::getCupCount());
    for (u32 i = 0; i < BattleCupSelectPageEx::getCupCount(); i++) {

        // Get button control variant
        char buffer[20];
        snprintf(buffer, sizeof(buffer), "Button%d", i);

        // Initialize the button
        PushButton* button = page->getCupButton(i);
        insertChild(i, button);
        button->load("button", "CupSelectCup", buffer, playerFlags, unk, false);
        button->buttonId = (int)i;

        // Set cup name
        const u16 cupIdx = CupManager::getCupIdxFromButton(i, page->curPage, true);
        const u16 msgId = CupManager::GetCup(cupIdx, true)->cupName;
        button->setText(msgId);

        // Set cup icon
        CupManager::updateCupButton(button, page->curPage, i, true);

        // Set button handlers
        button->setOnClickHandler(&onButtonClickHandler, 0);
        button->setOnSelectHandler(&onButtonSelectHandler);
    }

    // Set selection to first button
    page->setSelection(page->getCupButton(0));
}

// Select the cup
REPLACE void CtrlMenuBattleCupSelectCup::onButtonClick(PushButton* btn, u32 hudSlotId) {
    currentSelected = btn->buttonId;
    BattleCupSelectPageEx::getPage()->setCourse(this, btn, hudSlotId);
}

// Remove movie updating code
REPLACE void CtrlMenuBattleCupSelectCup::onButtonSelect(PushButton* btn, u32 hudSlotId) {
    currentSelected = btn->buttonId;
    BattleCupSelectPageEx::getPage()->setCourseNames(this, btn, hudSlotId);
}
