#include <kamek.h>
#include <game/ui/page/RaceCupSelectPage.h>
#include "cupsystem/CupManager.h"

// This function runs whenever the left arrow is selected or pressed
void RaceCupSelectArrow::onLeftArrowPress(SheetSelectControl* arrowPair, u32 localPlayerId) {

    // Get the parent screen
    RaceCupSelectPage* self = RaceCupSelectPage::getPage(Page::CUP_SELECT);

    // Update the page number
    self->extension.curPage--;
    if (self->extension.curPage < 0)
        self->extension.curPage = CupManager::getMaxCupPage(false);

    // Update each cup button
    for (int i = 0; i < 8; i++) {
        PushButton* cupButton = &self->cupHolder.cupButtons[i];
        u32 cupIdx = CupManager::getCupIdxFromButton(i, self->extension.curPage, false);
        cupButton->setMsgId(CupManager::GetCupArray(false)[cupIdx].cupName, NULL);
        CupManager::updateCupButton(i, cupButton, self->extension.curPage, false);
    }

    // Remove the text in the course selection
    for (int i = 0; i < 4; i++) {
        self->courseHolder.courseNames[i].resetText();
    }
}

// This function runs whenever the right arrow is selected or pressed
void RaceCupSelectArrow::onRightArrowPress(SheetSelectControl* arrowPair, u32 localPlayerId) {

    // Get the parent screen
    RaceCupSelectPage* self = RaceCupSelectPage::getPage(Page::CUP_SELECT);

    // Update the page number
    self->extension.curPage++;
    if (self->extension.curPage > CupManager::getMaxCupPage(false))
        self->extension.curPage = 0;

    // Update each cup button
    for (int i = 0; i < 8; i++) {
        PushButton* cupButton = &self->cupHolder.cupButtons[i];
        u32 cupIdx = CupManager::getCupIdxFromButton(i, self->extension.curPage, false);
        cupButton->setMsgId(CupManager::GetCupArray(false)[cupIdx].cupName, NULL);
        CupManager::updateCupButton(i, cupButton, self->extension.curPage, false);
    }

    // Remove the text in the course selection
    for (int i = 0; i < 4; i++) {
        self->courseHolder.courseNames[i].resetText();
    }
}
