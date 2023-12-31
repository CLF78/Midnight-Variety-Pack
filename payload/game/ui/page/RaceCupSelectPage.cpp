#include <common/Common.hpp>
#include <game/system/RaceConfig.hpp>
#include <game/system/SaveManager.hpp>
#include <game/ui/page/RaceCupSelectPage.hpp>
#include <game/ui/SectionManager.hpp>
#include <game/ui/UIUtils.hpp>
#include <midnight/cup/CupManager.hpp>

///////////////////////////////////
// Patches for Custom Cup System //
///////////////////////////////////

// Update memory size of page
kmCallDefCpp(0x80623D94, u32) {
    return sizeof(RaceCupSelectPage);
}

// Construct the expansion data
kmBranchDefCpp(0x80627A3C, NULL, RaceCupSelectPage*, RaceCupSelectPage* self) {

    // Update the children count
    if (CupManager::GetCupArrowsEnabled(false))
        self->layoutCount++;

    // Construct extra buttons
    SheetSelectControl::construct(&self->extension.arrows);

    // Set the input handlers
    InputHandler2<RaceCupSelectArrow, void, SheetSelectControl*, u32>::construct(
                                               (RaceCupSelectArrow*)&self->extension.arrows.leftButton,
                                               &RaceCupSelectArrow::onLeftArrowPress,
                                               &self->extension.leftHandler);
    self->extension.arrows.leftHandler = &self->extension.leftHandler;

    InputHandler2<RaceCupSelectArrow, void, SheetSelectControl*, u32>::construct(
                                                (RaceCupSelectArrow*)&self->extension.arrows.leftButton,
                                                &RaceCupSelectArrow::onRightArrowPress,
                                                &self->extension.rightHandler);
    self->extension.arrows.rightHandler = &self->extension.rightHandler;

    // Set the correct page
    s32 lastTrack = SectionManager::instance->globalContext->lastCourse;
    self->extension.curPage = CupManager::getCupPageFromTrack(lastTrack);
    return self;
}

// Destroy the expansion data
kmCallDefCpp(0x8084226C, void, RaceCupSelectPage* self) {

    // Only delete the arrows, since the other fields do not have/need a destructor
    self->extension.arrows.~SheetSelectControl();
}

// Load the arrows
kmBranchDefCpp(0x80841090, 0x808410F8, SheetSelectControl*, RaceCupSelectPage* page, int childIdx) {

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
    arrows->load("button", CUP_ARROW_R_BRCTR, rightVar,
                 CUP_ARROW_L_BRCTR, leftVar, 1, false, false);
    return arrows;
}

// RaceCupSelectPage::setCourse() override
// Set the selected track when a cup is clicked
kmCallDefCpp(0x807E5DA8, void, RaceCupSelectPage* self, CtrlMenuCupSelectCup* cupHolder, PushButton* button) {

    // Check for defocusing state
    if (self->pageState == Page::STATE_DEFOCUSING) {

        // Update selected button
        self->selectedButtonId = cupHolder->currentSelected;

        // Get the cup and its first track
        u32 cupIdx = CupManager::getCupIdxFromButton(self->selectedButtonId, self->extension.curPage);
        u32 trackIdx = CupManager::GetCupList()[cupIdx].entryId[0];

        // Get the previous cup, and update the last selected stage if it differs
        u32 prevCupIdx = CupManager::getCupIdxFromTrack(SectionManager::instance->globalContext->lastStage);
        if (cupIdx != prevCupIdx)
            SectionManager::instance->globalContext->lastStage = trackIdx;

        // Set the course data if offline
        if (!UIUtils::isOnlineRoom(SectionManager::instance->curSection->sectionID)) {

            // Store the cup id
            RaceConfig::instance->menuScenario.settings.cupId = cupIdx;

            // Get the actual track and store it
            u32 actualTrackIdx = CupManager::getTrackFile(trackIdx);
            CupManager::SetCourse(&RaceConfig::instance->menuScenario, actualTrackIdx);

            // If in GP mode, go straight to the OK button instead of the course selection
            if (RaceConfig::instance->menuScenario.settings.gameMode == RaceConfig::Settings::GAMEMODE_GP) {
                self->requestSectionChange(Section::DEMO_GP, button);
                return;
            }

        // Else wait for the course voting page to be loaded (is this even needed?)
        } else
            while (SectionManager::instance->curSection->pages[Page::WIFI_VOTING] == nullptr) {}

        // Go to the course select page
        self->startReplace(Page::COURSE_SELECT, button);
    }
}

// RaceCupSelectPage::updateTextMessages() override
// Display the GP rank from the expanded save
void RaceCupSelectPage::updateTextMessages(CtrlMenuCupSelectCup* cupHolder, u32 unk) {

    // Update course names
    u32 cupButtonId = cupHolder->currentSelected;
    this->courseHolder.setCourseNames(cupButtonId);

    // If game mode is Grand Prix, update the message at the bottom with the cup rank
    if (RaceConfig::instance->menuScenario.settings.gameMode == RaceConfig::Settings::GAMEMODE_GP) {

        // Initialize info
        MessageInfo msgInfo;

        // Get current license, bail if invalid
        SaveManager* save = SaveManager::instance;
        if (save->currentLicenseId == -1)
            return;

        // Get the cup entry
        SaveExpansion* licenseEx = &save->expansion.licensesEx[save->currentLicenseId];
        u32 cupId = CupManager::getCupIdxFromButton(cupButtonId, this->extension.curPage);
        SaveExpansion::Cup* cup = &licenseEx->gpRanks[cupId];

        // Get the message ID
        if (cup->completed)
            msgInfo.messageIds[0] = 3373 + cup->rank;
        else
            msgInfo.messageIds[0] = 3382;

        // Set it
        this->instructionText->setText(3360, &msgInfo);
    }
}

// Disable background movies
kmWrite16(0x80841F94, 0x4800);

///////////////////////////////////////////////////////////
// Patches for Custom Cup System + Custom Engine Classes //
///////////////////////////////////////////////////////////

// RaceCupSelectPage::onActivate() override
// Sets a lot of stuff
kmPointerDefCpp(0x808D9518, void, RaceCupSelectPage* self) {

    // Set top message
    if (RaceConfig::instance->menuScenario.settings.gameMode == RaceConfig::Settings::GAMEMODE_GP)
        self->titleBmgId = 3404;
    else
        self->titleBmgId = 3405;

    // Set default cup button to the cup the previously selected track belongs to
    if (self->animId == Page::ANIM_NEXT) {
        u32 lastTrack = SectionManager::instance->globalContext->lastCourse;
        self->selectedButtonId = CupManager::getCupButtonFromTrack(lastTrack, self->extension.curPage);
    }

    // Call base function
    self->callOnActivate();

    // Adjust X wrapping and arrow display by setting the distance function appropriately
    // 0 wraps on the X and Y axis, 1 wraps on Y axis only
    bool arrowsEnabled = CupManager::GetCupArrowsEnabled();
    int wrapType = (CupManager::GetCupCount() == 2 || arrowsEnabled);
    self->multiControlInputManager.setDistanceFunc(wrapType);

    // Disable the arrows if not required
    self->extension.arrows.leftButton.enabled = arrowsEnabled;
    self->extension.arrows.rightButton.enabled = arrowsEnabled;

    // Initialize cup holder
    self->cupHolder.init();

    // Load the Vote/Random popup if we are in an online room
    if (UIUtils::isOnlineRoom(SectionManager::instance->curSection->sectionID) && self->animId == Page::ANIM_NEXT) {

        // Get the page
        YesNoPopupPage* popupPage = YesNoPopupPage::getPage();

        // Reset it and update the messages
        popupPage->reset();
        popupPage->setWindowMessage(4350, nullptr);
        popupPage->configureButton(0, 4351, nullptr, Page::ANIM_NONE, nullptr);
        popupPage->configureButton(1, 4352, nullptr, Page::ANIM_NONE, nullptr);

        // Default to the Vote button
        popupPage->currSelected = 0;

        // Display the page and store a reference to it
        popupPage = (YesNoPopupPage*)self->addPage(Page::ONLINE_VOTE_PROMPT, Page::ANIM_NEXT);
        self->voteOrRandomPage = popupPage;
    }

    // Set the instruction text based on the game mode
    u32 msgId = 0;
    MessageInfo msgInfo;

    u32 gameMode = RaceConfig::instance->menuScenario.settings.gameMode;
    if (gameMode == RaceConfig::Settings::GAMEMODE_TT)
        msgId = 3361;
    else if (gameMode == RaceConfig::Settings::GAMEMODE_VS) {
        msgId = 3363;
        if (RaceConfig::instance->menuScenario.settings.modeFlags & RaceConfig::Settings::FLAG_TEAMS)
            msgId = 3366;
    }

    // Set CC argument
    switch (RaceConfig::instance->menuScenario.settings.engineClass) {

        case RaceConfig::Settings::CC_50:
            msgInfo.messageIds[0] = 3411;
            break;

        case RaceConfig::Settings::CC_100:
            msgInfo.messageIds[0] = 3412;
            break;

        case RaceConfig::Settings::CC_150:
            msgInfo.messageIds[0] = 3413;
            break;

        case RaceConfig::Settings::CC_200:
            msgInfo.messageIds[0] = 20003;
            break;

        case RaceConfig::Settings::CC_500:
            msgInfo.messageIds[0] = 20004;
            break;
    }

    // Set mirror argument (VS only)
    if (RaceConfig::instance->menuScenario.settings.modeFlags & RaceConfig::Settings::FLAG_MIRROR) {
        msgInfo.messageIds[1] = 3414;
        msgInfo.strings[0] = L" ";
    }

    // Apply message
    self->instructionText->setText(msgId, &msgInfo);
}
