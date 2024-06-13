#include <common/Common.hpp>
#include <game/ui/page/RaceCupSelectPage.hpp>
#include <midnight/cup/CupManager.hpp>

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
        u32 cupIdx = CupManager::getCupIdxFromButton(i, page->curPage);
        u16 msgId = CupManager::GetCupList()[cupIdx].cupName;
        button->setText(msgId, nullptr);

        // Set cup icon
        CupManager::updateCupButton(button, page->curPage, i);

        // Set button handlers
        button->setOnClickHandler(&onClickHandler, 0);
        button->setOnSelectHandler(&onSelectHandler);

        // Set as selected if the button is the previously selected one
        if (i == page->selectedButtonId) {
            page->setSelection(button); // is this needed?
            currentSelected = button->buttonId;
            page->updateTextMessages(this, 0);
        }
    }
}

// Update track names
REPLACE void CtrlMenuCupSelectCup::onCupSelect(PushButton* button, u32 unk) {
    currentSelected = button->buttonId;
    RaceCupSelectPageEx::getPage()->updateTextMessages(this, unk);
}

// Update courses
REPLACE void CtrlMenuCupSelectCup::onCupClick(PushButton* button, u32 unk) {
    currentSelected = button->buttonId;
    RaceCupSelectPageEx::getPage()->setCourse(this, button);
}
