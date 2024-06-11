#include <common/Common.hpp>
#include <game/ui/page/RaceCupSelectPage.hpp>
#include <midnight/cup/CupManager.hpp>
#include <midnight/cup/RaceCupSelectArrow.hpp>

// This function runs whenever the left arrow is selected or pressed
void RaceCupSelectArrow::onLeftArrowPress(SheetSelectControl* arrowPair, u32 localPlayerId) {

    // Get the parent page
    RaceCupSelectPage* self = RaceCupSelectPage::getPage();

    // Update the page number
    self->extension.curPage--;
    if (self->extension.curPage < 0)
        self->extension.curPage = CupManager::getMaxCupPage();

    // Update each cup button
    for (int i = 0; i < 8; i++) {
        PushButton* cupButton = &self->cupHolder.cupButtons[i];
        u32 cupIdx = CupManager::getCupIdxFromButton(i, self->extension.curPage);
        cupButton->setText(CupManager::GetCupList()[cupIdx].cupName, nullptr);
        CupManager::updateCupButton(cupButton, self->extension.curPage, i);
    }

    // Remove the text in the course selection
    for (int i = 0; i < 4; i++) {
        self->courseHolder.courseNames[i].resetText();
    }
}

// This function runs whenever the right arrow is selected or pressed
void RaceCupSelectArrow::onRightArrowPress(SheetSelectControl* arrowPair, u32 localPlayerId) {

    // Get the parent page
    RaceCupSelectPage* self = RaceCupSelectPage::getPage();

    // Update the page number
    self->extension.curPage++;
    if (self->extension.curPage > CupManager::getMaxCupPage())
        self->extension.curPage = 0;

    // Update each cup button
    for (int i = 0; i < 8; i++) {
        PushButton* cupButton = &self->cupHolder.cupButtons[i];
        u32 cupIdx = CupManager::getCupIdxFromButton(i, self->extension.curPage);
        cupButton->setText(CupManager::GetCupList()[cupIdx].cupName, nullptr);
        CupManager::updateCupButton(cupButton, self->extension.curPage, i);
    }

    // Remove the text in the course selection
    for (int i = 0; i < 4; i++) {
        self->courseHolder.courseNames[i].resetText();
    }
}
