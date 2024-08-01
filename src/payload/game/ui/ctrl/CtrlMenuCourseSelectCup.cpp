#include "CtrlMenuCourseSelectCup.hpp"
#include <mvp/cup/CupManager.hpp>
#include <mvp/cup/RaceCupSelectPageEx.hpp>

///////////////////////
// Custom Cup System //
///////////////////////

// Update cup names and icons
REPLACE void CtrlMenuCourseSelectCup::initSelf() {

    // Get page and selected cup
    RaceCupSelectPageEx* cupPage = RaceCupSelectPageEx::getPage();
    u32 selectedCup = cupPage->selectedButtonId;

    // Update each cup
    for (int i = 0; i < ARRAY_SIZE(cups); i++) {

        // Get cup
        CtrlMenuCourseSelectCupSub* cup = &cups[i];
        
        // Set name
        u32 cupIdx = CupManager::getCupIdxFromButton(i, cupPage->curPage);
        u16 cupName = CupManager::GetCupList()[cupIdx].cupName;
        cup->setText(cupName);

        // Set icon
        CupManager::updateCupButton(cup, cupPage->curPage, i);

        // Play switch animation
        cup->animator.getGroup(0)->setAnimation(0, 0.0f);
        cup->animator.getGroup(1)->setAnimation(i != selectedCup, 0.0f);
        cup->animator.getGroup(2)->setAnimation(i != selectedCup, 0.0f);
        cup->animator.getGroup(3)->setAnimation(i == selectedCup, 0.0f);

        // Call the virtual function
        cup->vf_3C(&elementPositions[POS_LAYOUT].trans);

        // Mark as selected if necessary
        cup->selected = (i == selectedCup);
    }
}
