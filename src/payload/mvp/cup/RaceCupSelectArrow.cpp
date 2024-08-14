#include "CupManager.hpp"
#include "RaceCupSelectArrow.hpp"
#include "RaceCupSelectPageEx.hpp"

// This function runs whenever the left arrow is selected or pressed
void RaceCupSelectArrow::onLeft(SheetSelectControl* arrowPair, u32 localPlayerId) {

    // Get the parent page
    RaceCupSelectPageEx* self = RaceCupSelectPageEx::getPage();

    // Update the page number
    self->curPage--;
    if (self->curPage < 0)
        self->curPage = CupManager::getMaxCupPage();

    // Log entry
    LOG_DEBUG("Changing cup select page to %d...", self->curPage);

    // Update each cup button
    for (int i = 0; i < 8; i++) {
        PushButton* cupButton = &self->cupHolder.cupButtons[i];
        const u32 cupIdx = CupManager::getCupIdxFromButton(i, self->curPage);
        cupButton->setText(CupManager::GetCupList()[cupIdx].cupName);
        CupManager::updateCupButton(cupButton, self->curPage, i);
    }

    // Remove the text in the course selection
    for (int i = 0; i < 4; i++) {
        self->courseHolder.courseNames[i].resetText();
    }
}

// This function runs whenever the right arrow is selected or pressed
void RaceCupSelectArrow::onRight(SheetSelectControl* arrowPair, u32 localPlayerId) {

    // Get the parent page
    RaceCupSelectPageEx* self = RaceCupSelectPageEx::getPage();

    // Update the page number
    self->curPage++;
    if (self->curPage > CupManager::getMaxCupPage())
        self->curPage = 0;

    // Log entry
    LOG_DEBUG("Changing cup select page to %d...", self->curPage);

    // Update each cup button
    for (int i = 0; i < 8; i++) {
        PushButton* cupButton = &self->cupHolder.cupButtons[i];
        const u32 cupIdx = CupManager::getCupIdxFromButton(i, self->curPage);
        cupButton->setText(CupManager::GetCupList()[cupIdx].cupName);
        CupManager::updateCupButton(cupButton, self->curPage, i);
    }

    // Remove the text in the course selection
    for (int i = 0; i < 4; i++) {
        self->courseHolder.courseNames[i].resetText();
    }
}

void RaceCupSelectArrow::onDeselect(u32 localPlayerId) {

    // Call the original function
    SheetSelectButton::onDeselect(localPlayerId);

    // Force select the previous selected cup
    RaceCupSelectPageEx* page = RaceCupSelectPageEx::getPage();
    const u32 curSelected = page->cupHolder.currentSelected;
    LOG_DEBUG("Restoring selected button to %d...", curSelected);
    page->cupHolder.cupButtons[curSelected].select(localPlayerId);
}
