#include <common/Common.hpp>
#include <game/ui/page/RaceCourseSelectPage.hpp>
#include <game/ui/page/RaceCupSelectPage.hpp>
#include <game/ui/page/VotingPage.hpp>
#include <game/ui/SectionManager.hpp>
#include <game/ui/UIUtils.hpp>
#include <game/system/RaceConfig.hpp>
#include <midnight/cup/CupManager.hpp>

///////////////////////////////////
// Patches for Custom Cup System //
///////////////////////////////////

// RaceCourseSelectPage::setCourse() override
// Set selected track
kmCallDefCpp(0x807E546C, void, RaceCourseSelectPage* self, CtrlMenuCourseSelectCourse* courseHolder, PushButton* button) {

    // Do not do anything if we're not defocusing the page
    if (self->pageState != Page::STATE_DEFOCUSING)
        return;

    // Get selected track and set it as last course
    RaceCupSelectPage* cupPage = RaceCupSelectPage::getPage();
    u32 cupIdx = CupManager::getCupIdxFromButton(cupPage->selectedButtonId, cupPage->extension.curPage);
    u32 trackIdx = CupManager::GetCupList()[cupIdx].entryId[button->buttonId];
    SectionManager::instance->globalContext->lastCourse = trackIdx;

    if (UIUtils::isOnlineRoom(SectionManager::instance->curSection->sectionID)) {

        // Submit the selected track
        VotingPage* votingPage = VotingPage::getPage();
        votingPage->submitVote(trackIdx);

        // Go to the next page
        self->nextPageId = Page::NONE;
        self->replace(Page::ANIM_NEXT, button->getDelay());

    } else {

        // Get the actual track to be played and store it
        u32 actualTrackIdx = CupManager::getTrackFile(trackIdx);
        CupManager::SetCourse(&RaceConfig::instance->menuScenario, actualTrackIdx);

        // If we're in TT mode, go to the ghost select screen
        u32 gameMode = RaceConfig::instance->menuScenario.settings.gameMode;
        if (gameMode == RaceConfig::Settings::GAMEMODE_TT)
            self->startReplace(Page::GHOST_SELECT_TOP, button);

        // If we're in VS mode, prepare intro and generate the track order from here
        else if (gameMode == RaceConfig::Settings::GAMEMODE_VS) {
            self->requestSectionChange(Section::DEMO_VS, button);
            CupManager::generateCourseOrder(cupIdx, button->buttonId);
        }
    }

    // Report course as selected
    self->courseSelected = true;
}

///////////////////////////////////////
// Patches for Custom Engine Classes //
///////////////////////////////////////

// RaceCourseSelectPage::onActivate() override
// Update instruction text with new CCs and Mirror option
kmPointerDefCpp(0x808D9480, void, RaceCourseSelectPage* self) {

    self->multiControlInputManager.setDistanceFunc(1);
    self->callOnActivate();

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
