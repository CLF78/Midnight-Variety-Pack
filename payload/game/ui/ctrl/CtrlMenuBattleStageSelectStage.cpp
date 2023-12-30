#include <common/Common.hpp>
#include <game/ui/ctrl/CtrlMenuBattleStageSelectStage.hpp>
#include <game/ui/page/BattleStageSelectPage.hpp>
#include <game/ui/page/BattleCupSelectPage.hpp>
#include <game/ui/SectionManager.hpp>
#include <midnight/cup/CupManager.hpp>

///////////////////////////////////
// Patches for Custom Cup System //
///////////////////////////////////

// CtrlMenuBattleStageSelectStage::onInit() override
// Update arena names
kmPointerDefCpp(0x808D2F18, void, CtrlMenuBattleStageSelectStage* self) {

    BattleCupSelectPage* cupPage = BattleCupSelectPage::getPage();
    BattleStageSelectPage* coursePage = BattleStageSelectPage::getPage();
    u32 selectedCup = cupPage->selectedButtonId;
    u32 lastStage = SectionManager::instance->globalContext->lastStage;
    int selected = -1;

    for (int i = 0; i < 5; i++) {

        // Get button
        PushButton* trackButton = &self->courseButtons[i];

        // Set name
        u32 cupIdx = CupManager::getCupIdxFromButton(selectedCup, cupPage->extension.curPage, true);
        u32 trackIdx = CupManager::GetCupList(true)[cupIdx].entryId[i];
        trackButton->setText(CupManager::getTrackName(trackIdx), nullptr);

        // Set button id
        trackButton->buttonId = i;

        // Play animation
        trackButton->animator.getGroup(4)->setAnimation(0, 0.0f);

        // Select the button if the track matches
        if (lastStage == trackIdx) {
            coursePage->setSelection(trackButton);
            selected = i;
        }
    }

    if (selected == -1)
        coursePage->setSelection((PushButton*)&self->courseButtons[0]);
}
