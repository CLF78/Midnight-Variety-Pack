#include <kamek.h>
#include <game/ui/page/RaceCupSelectPage.h>
#include "cupsystem/CupManager.h"
#if (CUSTOM_CUP_COURSE_SUPPORT && RACE_CUP_ARROWS_ENABLED)

// This function runs whenever the left arrow is selected or pressed
void RaceCupSelectArrow::onLeftArrowPress(SheetSelectControl* arrowPair, u32 localPlayerId) {

    // Get the parent screen
    RaceCupSelectPage* self = (RaceCupSelectPage*)MenuPage::getMenuPage(Page::CUP_SELECT);

    // Update the page number
    self->extension.curPage--;
    if (self->extension.curPage < 0)
        self->extension.curPage = CupManager::getMaxCupPage();

    // Update each cup button
    for (int i = 0; i < 8; i++) {
        PushButton* cupButton = &self->cupHolder.cupButtons[i];
        u32 cupIdx = CupManager::getCupIdxFromButton(i, self->extension.curPage);
        cupButton->setMsgId(CupFile::cups[cupIdx].cupName, NULL);
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

    // Update each cup button
    for (int i = 0; i < 8; i++) {
        PushButton* cupButton = &self->cupHolder.cupButtons[i];
        u32 cupIdx = CupManager::getCupIdxFromButton(i, self->extension.curPage);
        cupButton->setMsgId(CupFile::cups[cupIdx].cupName, NULL);
        CupManager::replaceCupIcon(i, cupButton, self->extension.curPage);
    }

    // Remove the text in the course selection
    for (int i = 0; i < 4; i++) {
        self->cupHolder.courseHolder.courseNames[i].resetText();
    }
}

#endif
