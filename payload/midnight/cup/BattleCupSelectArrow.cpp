#include <common/Common.hpp>
#include <game/ui/page/BattleCupSelectPage.hpp>
#include <midnight/cup/CupManager.hpp>

// This function runs whenever the left arrow is selected or pressed
void BattleCupSelectArrow::onLeftArrowPress(SheetSelectControl* arrowPair, u32 localPlayerId) {

    // Get the parent page
    BattleCupSelectPage* self = BattleCupSelectPage::getPage();

    // Update the page number
    self->extension.curPage--;
    if (self->extension.curPage < 0)
        self->extension.curPage = CupManager::getMaxCupPage(true);

    // Update each cup button
    for (int i = 0; i < 8; i++) {
        PushButton* cupButton = self->getCupButton(i);
        u32 cupIdx = CupManager::getCupIdxFromButton(i, self->extension.curPage, true);
        cupButton->setText(CupManager::GetCupList(true)[cupIdx].cupName, nullptr);
        CupManager::updateCupButton(cupButton, self->extension.curPage, i, true);
    }

    // Remove the text in the course selection
    for (int i = 0; i < 5; i++) {
        self->stageHolder.courseNames[i].resetText();
    }
}

// This function runs whenever the right arrow is selected or pressed
void BattleCupSelectArrow::onRightArrowPress(SheetSelectControl* arrowPair, u32 localPlayerId) {

    // Get the parent page
    BattleCupSelectPage* self = BattleCupSelectPage::getPage();

    // Update the page number
    self->extension.curPage++;
    if (self->extension.curPage > CupManager::getMaxCupPage(true))
        self->extension.curPage = 0;

    // Update each cup button
    for (int i = 0; i < 8; i++) {
        PushButton* cupButton = self->getCupButton(i);
        u32 cupIdx = CupManager::getCupIdxFromButton(i, self->extension.curPage, true);
        cupButton->setText(CupManager::GetCupList(true)[cupIdx].cupName, nullptr);
        CupManager::updateCupButton(cupButton, self->extension.curPage, i, true);
    }

    // Remove the text in the course selection
    for (int i = 0; i < 5; i++) {
        self->stageHolder.courseNames[i].resetText();
    }
}
