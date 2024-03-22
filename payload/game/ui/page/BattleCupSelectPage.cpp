#include <common/Common.hpp>
#include <game/ui/page/BattleCupSelectPage.hpp>
#include <game/ui/SectionManager.hpp>
#include <game/ui/UIUtils.hpp>
#include <midnight/cup/BattleCupSelectArrow.hpp>
#include <midnight/cup/CupManager.hpp>

///////////////////////
// Custom Cup System //
///////////////////////

// Section::createPage() patch (here for convenience)
// Update memory size of page
kmCallDefCpp(0x80623E84, u32) {
    return sizeof(BattleCupSelectPage);
}

// BattleCupSelectPage::BattleCupSelectPage() patch
// Construct the expansion data
kmBranchDefCpp(0x80629854, NULL, BattleCupSelectPage*, BattleCupSelectPage* self) {

    // Update the children count
    self->layoutCount++;

    // Construct the extra cup buttons
    for (int i = 0; i < ARRAY_SIZE(self->extension.cupButtons); i++)
        PushButton::construct(&self->extension.cupButtons[i]);

    // Construct the arrows
    SheetSelectControl::construct(&self->extension.arrows);

    // Set the input handlers
    InputHandler2<BattleCupSelectArrow, void, SheetSelectControl*, u32>::construct(
                                               (BattleCupSelectArrow*)&self->extension.arrows.leftButton,
                                               &BattleCupSelectArrow::onLeftArrowPress,
                                               &self->extension.leftHandler);
    self->extension.arrows.leftHandler = &self->extension.leftHandler;

    InputHandler2<BattleCupSelectArrow, void, SheetSelectControl*, u32>::construct(
                                                (BattleCupSelectArrow*)&self->extension.arrows.leftButton,
                                                &BattleCupSelectArrow::onRightArrowPress,
                                                &self->extension.rightHandler);
    self->extension.arrows.rightHandler = &self->extension.rightHandler;

    // Set the correct page
    s32 lastTrack = SectionManager::instance->globalContext->lastStage;
    self->extension.curPage = CupManager::getCupPageFromTrack(lastTrack, true);
    return self;
}

// BattleCupSelectPage::loadLayout() patch
// Load the arrows
kmCallDefCpp(0x8083906C, SheetSelectControl*, BattleCupSelectPage* page, int childIdx) {

    if (childIdx != 2)
        return nullptr;

    // Insert entry
    SheetSelectControl* arrows = &page->extension.arrows;
    page->insertChild(childIdx, arrows, 0);

    // Determine the variant to use depending on the player count
    const char* rightVar = "ButtonArrowRight";
    const char* leftVar = "ButtonArrowLeft";
    if (UIUtils::getPlayerCount() > 2) {
        rightVar = "ButtonArrowRight2";
        leftVar = "ButtonArrowLeft2";
    }

    // Load BRCTR
    arrows->load("button", CUP_ARROW_R_BRCTR, rightVar, CUP_ARROW_L_BRCTR, leftVar, 1, false, false);
    return arrows;
}

// BattleCupSelectPage::onActivate() patch
// Set the starting cup button, skip the boundary check, adjust selection wrapping and disable the
// arrows if not required
kmBranchDefCpp(0x808390A4, 0x808390D8, void, BattleCupSelectPage* self) {

    // Set the starting button
    u32 lastStage = SectionManager::instance->globalContext->lastStage;
    self->selectedButtonId = CupManager::getCupButtonFromTrack(lastStage, self->extension.curPage, true);

    // Call overloaded function
    self->callOnActivate();

    // Adjust X wrapping by setting the correct distance function
    // 0 wraps on the X and Y axis, 1 wraps on Y axis only
    bool arrowsEnabled = CupManager::GetCupArrowsEnabled(true);
    int wrapType = (CupManager::GetCupCount(true) == 2 || arrowsEnabled);
    self->multiControlInputManager.setDistanceFunc(wrapType);

    // Disable the arrows if not required
    self->extension.arrows.configure(arrowsEnabled, arrowsEnabled);
}

// BattleCupSelectPage::setCourse() override
// Set the selected stage when a cup is clicked
kmBranchDefCpp(0x8083955C, NULL, void, BattleCupSelectPage* self, CtrlMenuBattleCupSelectCup* cupHolder, PushButton* button) {

    // Check for defocusing state
    if (self->pageState == Page::STATE_DEFOCUSING) {

        // Update selected button
        self->selectedButtonId = cupHolder->currentSelected;

        // Get the cup and its first track
        u32 cupIdx = CupManager::getCupIdxFromButton(self->selectedButtonId, self->extension.curPage, true);
        u32 trackIdx = CupManager::GetCupList(true)[cupIdx].entryId[0];

        // Get the previous cup, and update the last selected stage if it differs
        u32 prevCupIdx = CupManager::getCupIdxFromTrack(SectionManager::instance->globalContext->lastStage, true);
        if (cupIdx != prevCupIdx)
            SectionManager::instance->globalContext->lastStage = trackIdx;

        // Set the course data if offline
        if (!UIUtils::isOnlineRoom(SectionManager::instance->curSection->sectionID)) {

            // Get the actual track and store it
            u32 actualTrackIdx = CupManager::getTrackFile(trackIdx);
            CupManager::SetCourse(&RaceConfig::instance->menuScenario.settings, actualTrackIdx);
        
        // Else wait for the course voting page to be loaded (is this even needed?)
        } else {
            while (SectionManager::instance->curSection->pages[Page::WIFI_VOTING] == nullptr) {}
        }

        // Go to the course select page
        self->startReplace(Page::COURSE_SELECT_BT, button);
    }
}

// BattleCupSelectPage::~BattleCupSelectPage() patch
// Destroy the expansion data
kmCallDefCpp(0x80839FEC, void, BattleCupSelectPage* self) {

    // Delete the cup buttons
    for (int i = 0; i < ARRAY_SIZE(self->extension.cupButtons); i++)
        self->extension.cupButtons[i].~PushButton();

    // Delete the arrows
    self->extension.arrows.~SheetSelectControl();
}
