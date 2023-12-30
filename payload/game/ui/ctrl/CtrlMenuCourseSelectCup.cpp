#include <common/Common.hpp>
#include <game/ui/page/RaceCourseSelectPage.hpp>
#include <game/ui/page/RaceCupSelectPage.hpp>
#include <game/ui/ctrl/CtrlMenuCourseSelectCup.hpp>
#include <midnight/cup/CupManager.hpp>

///////////////////////////////////
// Patches for Custom Cup System //
///////////////////////////////////

// CtrlMenuCourseSelectCup::onInit() override
// Update cup names and icons
kmPointerDefCpp(0x808D3190, void, CtrlMenuCourseSelectCup* self) {

    // Get pages and selected cup
    RaceCupSelectPage* cupPage = RaceCupSelectPage::getPage();
    RaceCourseSelectPage* coursePage = RaceCourseSelectPage::getPage();
    u32 selectedCup = cupPage->selectedButtonId;

    // Update each cup
    for (int i = 0; i < 8; i++) {

        // Get cup
        CtrlMenuCourseSelectCupSub* cup = &coursePage->cupHolder.cups[i];
        
        // Set name
        u32 cupIdx = CupManager::getCupIdxFromButton(i, cupPage->extension.curPage);
        u16 cupName = CupManager::GetCupList()[cupIdx].cupName;
        cup->setText(cupName, nullptr);

        // Set icon
        CupManager::updateCupButton(cup, cupPage->extension.curPage, i);

        // Play switch animation
        cup->animator.getGroup(0)->setAnimation(0, 0.0f);
        cup->animator.getGroup(1)->setAnimation(i != selectedCup, 0.0f);
        cup->animator.getGroup(2)->setAnimation(i != selectedCup, 0.0f);
        cup->animator.getGroup(3)->setAnimation(i == selectedCup, 0.0f);

        // Call the virtual function
        cup->vf_3C(&self->elementPositions[POS_LAYOUT].trans);

        // Mark as selected if necessary
        cup->selected = (i == selectedCup);
    }
}
