#include <common/Common.hpp>
#include <mvp/cup/BattleCupSelectPageEx.hpp>
#include <mvp/cup/BattleCupSelectArrow.hpp>
#include <mvp/cup/CupManager.hpp>

// This function runs whenever the left arrow is selected or pressed
void BattleCupSelectArrow::onLeftArrowPress(SheetSelectControl* arrowPair, u32 localPlayerId) {

    // Get the parent page
    BattleCupSelectPageEx* page = BattleCupSelectPageEx::getPage();

    // Update the page number
    page->curPage--;
    if (page->curPage < 0)
        page->curPage = CupManager::getMaxCupPage(true);

    // Log entry
    LOG_DEBUG("Changing cup select page to %d...", page->curPage);

    // Update each cup button
    for (int i = 0; i < 8; i++) {
        PushButton* cupButton = page->getCupButton(i);
        u32 cupIdx = CupManager::getCupIdxFromButton(i, page->curPage, true);
        cupButton->setText(CupManager::GetCupList(true)[cupIdx].cupName);
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

    // Log entry
    LOG_DEBUG("Changing cup select page to %d...", page->curPage);

    // Update each cup button
    for (int i = 0; i < 8; i++) {
        PushButton* cupButton = page->getCupButton(i);
        u32 cupIdx = CupManager::getCupIdxFromButton(i, page->curPage, true);
        cupButton->setText(CupManager::GetCupList(true)[cupIdx].cupName);
        CupManager::updateCupButton(cupButton, page->curPage, i, true);
    }

    // Remove the text in the course selection
    for (int i = 0; i < 4; i++) {
        page->stageHolder.courseNames[i].resetText();
    }
}

void BattleCupSelectArrow::onDeselect(u32 localPlayerId) {

    // Call the original function
    SheetSelectButton::onDeselect(localPlayerId);

    // Force select the previous selected cup
    BattleCupSelectPageEx* page = BattleCupSelectPageEx::getPage();
    u32 curSelected = page->cupHolder.currentSelected;
    LOG_DEBUG("Restoring selected button to %d...", curSelected);
    page->getCupButton(curSelected)->select(localPlayerId);
}
