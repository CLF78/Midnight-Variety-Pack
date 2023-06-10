#include <kamek.h>
#include <game/ui/page/BattleCupSelectPage.h>
#include "cupsystem/CupManager.h"

// This function runs whenever the left arrow is selected or pressed
void BattleCupSelectArrow::onLeftArrowPress(SheetSelectControl* arrowPair, u32 localPlayerId) {

    // Get the parent screen
    BattleCupSelectPage* self = BattleCupSelectPage::getPage(Page::CUP_SELECT_BT);

    // Update the page number
    self->extension.curPage--;
    if (self->extension.curPage < 0)
        self->extension.curPage = CupManager::getMaxCupPage();

    // Update each cup button
    for (int i = 0; i < 8; i++) {
        PushButton* cupButton = self->getCupButton(i);
        u32 cupIdx = CupManager::getCupIdxFromButton(i, self->extension.curPage);
        cupButton->setMsgId(CupManager::GetCupArray()[cupIdx].cupName, NULL);
        CupManager::updateCupButton(i, cupButton, self->extension.curPage);
    }

    // Remove the text in the course selection
    for (int i = 0; i < 5; i++) {
        self->stageHolder.courseNames[i].resetText();
    }
}

// This function runs whenever the right arrow is selected or pressed
void BattleCupSelectArrow::onRightArrowPress(SheetSelectControl* arrowPair, u32 localPlayerId) {

    // Get the parent screen
    BattleCupSelectPage* self = BattleCupSelectPage::getPage(Page::CUP_SELECT_BT);

    // Update the page number
    self->extension.curPage++;
    if (self->extension.curPage > CupManager::getMaxCupPage())
        self->extension.curPage = 0;

    // Update each cup button
    for (int i = 0; i < 8; i++) {
        PushButton* cupButton = self->getCupButton(i);
        u32 cupIdx = CupManager::getCupIdxFromButton(i, self->extension.curPage);
        cupButton->setMsgId(CupManager::GetCupArray()[cupIdx].cupName, NULL);
        CupManager::updateCupButton(i, cupButton, self->extension.curPage);
    }

    // Remove the text in the course selection
    for (int i = 0; i < 5; i++) {
        self->stageHolder.courseNames[i].resetText();
    }
}
