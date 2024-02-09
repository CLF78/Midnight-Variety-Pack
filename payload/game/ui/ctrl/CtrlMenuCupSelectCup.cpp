#include <common/Common.hpp>
#include <game/ui/page/RaceCupSelectPage.hpp>
#include <midnight/cup/CupManager.hpp>

///////////////////////
// Custom Cup System //
///////////////////////

// CtrlMenuCupSelectCup::initSelf() override
// Update cup names and icons
kmPointerDefCpp(0x808D324C, void, CtrlMenuCupSelectCup* self) {

    // Get owning page
    RaceCupSelectPage* page = RaceCupSelectPage::getPage();

    // Update children
    for (int i = 0; i < 8; i++) {

        // Get the button
        PushButton* button = &page->cupHolder.cupButtons[i];

        // Set cup name
        u32 cupIdx = CupManager::getCupIdxFromButton(i, page->extension.curPage);
        u16 msgId = CupManager::GetCupList()[cupIdx].cupName;
        button->setText(msgId, nullptr);

        // Set cup icon
        CupManager::updateCupButton(button, page->extension.curPage, i);

        // Set button handlers
        button->setOnClickHandler(&self->onClickHandler,0);
        button->setOnSelectHandler(&self->onSelectHandler);

        // Set as selected if the button is the previously selected one
        if (i == page->selectedButtonId) {
            page->setSelection(button); // is this needed?
            self->currentSelected = button->buttonId;
            page->updateTextMessages(self, 0);
        }
    }
}

// CtrlMenuCupSelectCup::onSelect() override
// Update track names
kmPointerDefCpp(0x808BC104, void, CtrlMenuCupSelectCup* self, PushButton* button, u32 unk) {
    self->currentSelected = button->buttonId;
    RaceCupSelectPage* page = RaceCupSelectPage::getPage();
    page->updateTextMessages(self, unk);
}
