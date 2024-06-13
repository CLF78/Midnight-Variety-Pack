#include <common/Common.hpp>
#include <game/ui/page/RaceCupSelectPage.hpp>
#include <midnight/cup/CupManager.hpp>
#include <midnight/cup/RaceCupSelectArrow.hpp>

// This function runs whenever the left arrow is selected or pressed
void RaceCupSelectArrow::onLeftArrowPress(SheetSelectControl* arrowPair, u32 localPlayerId) {

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
        u32 cupIdx = CupManager::getCupIdxFromButton(i, self->curPage);
        cupButton->setText(CupManager::GetCupList()[cupIdx].cupName, nullptr);
        CupManager::updateCupButton(cupButton, self->curPage, i);
    }

    // Remove the text in the course selection
    for (int i = 0; i < 4; i++) {
        self->courseHolder.courseNames[i].resetText();
    }
}

// This function runs whenever the right arrow is selected or pressed
void RaceCupSelectArrow::onRightArrowPress(SheetSelectControl* arrowPair, u32 localPlayerId) {

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
        u32 cupIdx = CupManager::getCupIdxFromButton(i, self->curPage);
        cupButton->setText(CupManager::GetCupList()[cupIdx].cupName, nullptr);
        CupManager::updateCupButton(cupButton, self->curPage, i);
    }

    // Remove the text in the course selection
    for (int i = 0; i < 4; i++) {
        self->courseHolder.courseNames[i].resetText();
    }
}
