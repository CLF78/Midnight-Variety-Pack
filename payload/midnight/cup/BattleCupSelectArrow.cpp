#include <common/Common.hpp>
#include <game/ui/page/BattleCupSelectPage.hpp>
#include <midnight/cup/BattleCupSelectArrow.hpp>
#include <midnight/cup/CupManager.hpp>

// This function runs whenever the left arrow is selected or pressed
void BattleCupSelectArrow::onLeftArrowPress(SheetSelectControl* arrowPair, u32 localPlayerId) {

    // Get the parent page
    BattleCupSelectPageEx* page = BattleCupSelectPageEx::getPage();

    // Update the page number
    page->curPage--;
    if (page->curPage < 0)
        page->curPage = CupManager::getMaxCupPage(true);

    // Update each cup button
    for (int i = 0; i < 8; i++) {
        PushButton* cupButton = page->getCupButton(i);
        u32 cupIdx = CupManager::getCupIdxFromButton(i, page->curPage, true);
        cupButton->setText(CupManager::GetCupList(true)[cupIdx].cupName, nullptr);
        CupManager::updateCupButton(cupButton, page->curPage, i, true);
    }

    // Remove the text in the course selection
    for (int i = 0; i < 4; i++) {
        page->stageHolder.courseNames[i].resetText();
    }
}

// This function runs whenever the right arrow is selected or pressed
void BattleCupSelectArrow::onRightArrowPress(SheetSelectControl* arrowPair, u32 localPlayerId) {

    // Get the parent page
    BattleCupSelectPageEx* page = BattleCupSelectPageEx::getPage();

    // Update the page number
    page->curPage++;
    if (page->curPage > CupManager::getMaxCupPage(true))
        page->curPage = 0;

    // Update each cup button
    for (int i = 0; i < 8; i++) {
        PushButton* cupButton = page->getCupButton(i);
        u32 cupIdx = CupManager::getCupIdxFromButton(i, page->curPage, true);
        cupButton->setText(CupManager::GetCupList(true)[cupIdx].cupName, nullptr);
        CupManager::updateCupButton(cupButton, page->curPage, i, true);
    }

    // Remove the text in the course selection
    for (int i = 0; i < 4; i++) {
        page->stageHolder.courseNames[i].resetText();
    }
}
