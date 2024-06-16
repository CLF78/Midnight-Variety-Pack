#include <common/Common.hpp>
#include <game/ui/Message.hpp>
#include <game/ui/SectionManager.hpp>
#include <game/ui/UIUtils.hpp>
#include <game/ui/page/RaceCourseSelectPage.hpp>
#include <game/ui/page/VotingPage.hpp>
#include <game/system/RaceConfig.hpp>
#include <midnight/cup/CupManager.hpp>
#include <midnight/cup/RaceCupSelectPageEx.hpp>

///////////////////////
// Custom Cup System //
///////////////////////

// Set the selected track
REPLACE void RaceCourseSelectPage::setCourse(CtrlMenuCourseSelectCourse* courseHolder, PushButton* button) {

    // Do not do anything if we're not defocusing the page
    if (pageState != Page::STATE_DEFOCUSING)
        return;

    // Get selected track and set it as last course
    RaceCupSelectPageEx* cupPage = RaceCupSelectPageEx::getPage();
    u32 cupIdx = CupManager::getCupIdxFromButton(cupPage->selectedButtonId, cupPage->curPage);
    u32 trackIdx = CupManager::GetCupList()[cupIdx].entryId[button->buttonId];
    SectionManager::instance->globalContext->lastCourse = trackIdx;

    if (UIUtils::isOnlineRoom(SectionManager::instance->curSection->sectionID)) {

        // Submit the selected track
        VotingPage* votingPage = VotingPage::getPage();
        votingPage->submitVote(trackIdx);

        // Go to the next page
        nextPageId = Page::NONE;
        replace(Page::ANIM_NEXT, button->getDelay());

    } else {

        // Get the actual track to be played and store it
        u32 actualTrackIdx = CupManager::getTrackFile(trackIdx);
        CupManager::SetCourse(&RaceConfig::instance->menuScenario.settings, actualTrackIdx);

        // If we're in TT mode, go to the ghost select screen
        u32 gameMode = RaceConfig::instance->menuScenario.settings.gameMode;
        if (gameMode == RaceConfig::Settings::GAMEMODE_TT)
            startReplace(Page::GHOST_SELECT_TOP, button);

        // If we're in VS mode, prepare intro and generate the track order from here
        else if (gameMode == RaceConfig::Settings::GAMEMODE_VS) {
            requestSectionChange(Section::DEMO_VS, button);
            CupManager::generateCourseOrder(cupIdx, button->buttonId);
        }
    }

    // Report course as selected
    courseSelected = true;
}

///////////////////////////
// Custom Engine Classes //
///////////////////////////

// Update instruction text with new CCs and Mirror option
REPLACE void RaceCourseSelectPage::onActivate() {

    multiControlInputManager.setDistanceFunc(MultiControlInputManager::Y_WRAP);
    MenuPage::onActivate();

    // Set the instruction text based on the game mode
    u32 msgId = Message::NONE;
    MessageInfo msgInfo;
    switch(RaceConfig::instance->menuScenario.settings.gameMode) {

        case RaceConfig::Settings::GAMEMODE_TT:
            msgId = Message::Menu::INSTRUCTION_TEXT_TIME_TRIAL;
            break;

        case RaceConfig::Settings::GAMEMODE_VS:
            msgId = (RaceConfig::instance->menuScenario.settings.modeFlags & RaceConfig::Settings::FLAG_TEAMS) ?
                    Message::Menu::INSTRUCTION_TEXT_TEAM_VS :
                    Message::Menu::INSTRUCTION_TEXT_VS;
            break;

        default:
            break;
    }

    // Set CC argument
    switch (RaceConfig::instance->menuScenario.settings.engineClass) {

        case RaceConfig::Settings::CC_50:
            msgInfo.messageIds[0] = Message::Menu::CC_50;
            break;

        case RaceConfig::Settings::CC_100:
            msgInfo.messageIds[0] = Message::Menu::CC_100;
            break;

        case RaceConfig::Settings::CC_150:
            msgInfo.messageIds[0] = Message::Menu::CC_150;
            break;

        case RaceConfig::Settings::CC_200:
            msgInfo.messageIds[0] = Message::Menu::CC_200;
            break;

        case RaceConfig::Settings::CC_500:
            msgInfo.messageIds[0] = Message::Menu::CC_500;
            break;
    }

    // Set mirror argument and apply message
    msgInfo.setCondMessageValue(0, RaceConfig::instance->menuScenario.settings.modeFlags & RaceConfig::Settings::FLAG_MIRROR != 0, true);
    instructionText->setText(msgId, &msgInfo);
}
