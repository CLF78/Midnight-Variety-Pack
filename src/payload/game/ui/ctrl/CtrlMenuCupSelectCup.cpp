#include "CtrlMenuCupSelectCup.hpp"
#include <mvp/cup/CupManager.hpp>
#include <mvp/cup/RaceCupSelectPageEx.hpp>

///////////////////////
// Custom Cup System //
///////////////////////

// Update cup names and icons
REPLACE void CtrlMenuCupSelectCup::initSelf() {

    // Get owning page
    RaceCupSelectPageEx* page = RaceCupSelectPageEx::getPage();

    // Update children
    for (int i = 0; i < ARRAY_SIZE(cupButtons); i++) {

        // Get the button
        PushButton* button = &cupButtons[i];

        // Set cup name
        const u32 cupIdx = CupManager::getCupIdxFromButton(i, page->curPage);
        const u16 msgId = CupManager::GetCupList()[cupIdx].cupName;
        button->setText(msgId);

        // Set cup icon
        CupManager::updateCupButton(button, page->curPage, i);

        // Set button handlers
        button->setOnClickHandler(&onButtonClickHandler, 0);
        button->setOnSelectHandler(&onButtonSelectHandler);

        // Set as selected if the button is the previously selected one
        if (i == page->selectedButtonId) {
            page->setSelection(button); // is this needed?
            currentSelected = button->buttonId;
            page->updateTextMessages(this, 0);
        }
    }
}

// Update track names
REPLACE void CtrlMenuCupSelectCup::onButtonSelect(PushButton* button, u32 hudSlotId) {
    currentSelected = button->buttonId;
    RaceCupSelectPageEx::getPage()->updateTextMessages(this, hudSlotId);
}

// Update courses
REPLACE void CtrlMenuCupSelectCup::onButtonClick(PushButton* button, u32 hudSlotId) {
    currentSelected = button->buttonId;
    RaceCupSelectPageEx::getPage()->setCourse(this, button);
}
