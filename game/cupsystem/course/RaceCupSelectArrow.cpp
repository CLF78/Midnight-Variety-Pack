#include <kamek.h>
#include <game/ui/page/RaceCupSelectPage.h>
#include "cupsystem/CupManager.h"
#if (CUSTOM_CUP_SYSTEM && CUSTOM_CUP_COURSE_SUPPORT && CUP_COUNT > 8)

// This function runs whenever the left arrow is selected or pressed
void RaceCupSelectArrow::onLeftArrowPress(SheetSelectControl* arrowPair, u32 localPlayerId) {

    // Get the parent screen
    RaceCupSelectPage* self = (RaceCupSelectPage*)MenuPage::getMenuPage(Page::CUP_SELECT);

    // Update the page number
    self->extension.curPage--;
    if (self->extension.curPage < 0)
        self->extension.curPage = CupManager::getMaxCupPage();

    // Update each button
    for (int i = 0; i < 8; i++) {
        PushButton* cupButton = &self->cupHolder.cupButtons[i];
        u16 cupName = CupManager::getCupNameFromButton(i, self->extension.curPage);
        cupButton->setMsgId(cupName, NULL);
        CupManager::replaceCupIcon(i, cupButton, self->extension.curPage);
    }

    // Remove the text in the course selection
    for (int i = 0; i < 4; i++) {
        self->cupHolder.courseHolder.courseNames[i].resetText();
    }
}

// This function runs whenever the right arrow is selected or pressed
void RaceCupSelectArrow::onRightArrowPress(SheetSelectControl* arrowPair, u32 localPlayerId) {

    // Get the parent screen
    RaceCupSelectPage* self = (RaceCupSelectPage*)MenuPage::getMenuPage(Page::CUP_SELECT);

    // Update the page number
    self->extension.curPage++;
    if (self->extension.curPage > CupManager::getMaxCupPage())
        self->extension.curPage = 0;

    // Update each button
    for (int i = 0; i < 8; i++) {
        PushButton* cupButton = &self->cupHolder.cupButtons[i];
        u16 cupName = CupManager::getCupNameFromButton(i, self->extension.curPage);
        cupButton->setMsgId(cupName, NULL);
        CupManager::replaceCupIcon(i, cupButton, self->extension.curPage);
    }

    // Remove the text in the course selection
    for (int i = 0; i < 4; i++) {
        self->cupHolder.courseHolder.courseNames[i].resetText();
    }
}

#endif
