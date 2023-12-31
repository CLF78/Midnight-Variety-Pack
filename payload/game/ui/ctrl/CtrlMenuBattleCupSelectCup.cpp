#include <common/Common.hpp>
#include <game/ui/page/BattleCupSelectPage.hpp>
#include <game/ui/ControlLoader.hpp>
#include <game/ui/UIUtils.hpp>
#include <midnight/cup/CupManager.hpp>
#include <platform/stdio.h>

///////////////////////////////////
// Patches for Custom Cup System //
///////////////////////////////////

// CtrlMenuBattleCupSelectCup::initSelf() override
// Remove the loop, the cup boundary check and more unnecessary crud
kmPointerDefCpp(0x808D2EDC, void, CtrlMenuBattleCupSelectCup* self) {

    // Get page
    BattleCupSelectPage* page = BattleCupSelectPage::getPage();

    // Replace loop with a straight function
    // This does not have any safeguards and assumes the selectedButtonId value is valid
    PushButton* cupButton = page->getCupButton(page->selectedButtonId);
    page->setSelection(cupButton); // is this needed?
    self->currentSelected = cupButton->buttonId;
    page->setCourseNames(self, cupButton, 0);
}

// CtrlMenuBattleCupSelectCup::load() override
// Replace the BRCTR, update the button loop size, set cup names and icons and remove the background movies
kmCallDefCpp(0x80839030, void, CtrlMenuBattleCupSelectCup* self, u32 playerFlags, bool unk) {

    // Initialize loader and get page
    ControlLoader loader(self);
    BattleCupSelectPage* page = BattleCupSelectPage::getPage();

    // Set starting button to the first one
    self->currentSelected = 0;

    // Load the main controller
    u8 playerCount = UIUtils::getPlayerCount();
    const char* mainCtr = (playerCount <= 2) ? "CupSelectCupNULL" : "CupSelectCupNULL_4";
    loader.load("control", "CupSelectNULL", mainCtr, nullptr);

    // Initialize children
    self->initChildren(8);
    for (int i = 0; i < 8; i++) {

        // Get button control variant
        char buffer[20];
        snprintf(buffer, sizeof(buffer), "Button%d", i);

        // Initialize the button
        PushButton* button = page->getCupButton(i);
        self->insertChild(i, button);
        button->load("button", "CupSelectCup", buffer, playerFlags, unk, false);
        button->buttonId = i;

        // Set cup name
        u32 cupIdx = CupManager::getCupIdxFromButton(i, page->extension.curPage, true);
        u16 msgId = CupManager::GetCupList(true)[cupIdx].cupName;
        button->setText(msgId, nullptr);

        // Set cup icon
        CupManager::updateCupButton(button, page->extension.curPage, i, true);

        // Set button handlers
        button->setOnClickHandler(&self->onClickHandler,0);
        button->setOnSelectHandler(&self->onSelectHandler);
    }

    // Set selection to first button
    page->setSelection(page->getCupButton(0));
}

// Prevent uninitialized movie layout from crashing the game
kmWrite32(0x807E0D3C, 0x60000000);
