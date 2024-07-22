#include <common/Common.hpp>
#include <game/ui/Message.hpp>
#include <game/ui/SectionManager.hpp>
#include <game/ui/UIUtils.hpp>
#include <game/ui/page/VotingPage.hpp>
#include <game/system/RaceConfig.hpp>
#include <mvp/cup/CupManager.hpp>
#include <mvp/cup/RaceCupSelectPageEx.hpp>
#include <mvp/cup/RaceCourseSelectPageEx.hpp>
#include <mvp/online/RepickQueue.hpp>

///////////////////////
// Custom Cup System //
///////////////////////

void RaceCourseSelectPageEx::onRepickPromptPress(s32 choice, PushButton* button) {

    // If Vote Anyway is selected, go to the next page
    if (choice == 0) {
        nextPageId = Page::NONE;
        replace(Page::ANIM_NEXT, button->getDelay());
    }

    // Remove the prompt reference anyway
    repickPrompt = nullptr;
}

// Set the selected track
void RaceCourseSelectPageEx::setCourse(CtrlMenuCourseSelectCourse* courseHolder, PushButton* button, int unk) {

    // Check for active state
    if (pageState != Page::STATE_ACTIVE)
        return;

    // Get selected track and set it as last course
    RaceCupSelectPageEx* cupPage = RaceCupSelectPageEx::getPage();
    u32 cupIdx = CupManager::getCupIdxFromButton(cupPage->selectedButtonId, cupPage->curPage);
    u32 trackIdx = CupManager::GetCupList()[cupIdx].entryId[button->buttonId];
    SectionManager::instance->globalContext->lastCourse = trackIdx;

    if (UIUtils::isOnlineRoom(SectionManager::instance->curSection->sectionId)) {

        // Submit the selected track
        VotingPage* votingPage = VotingPage::getPage();
        votingPage->submitVote(trackIdx);

        // If the track is in the repick queue, show a popup
        if (RepickQueue::instance.GetQueuePosition(trackIdx) != RepickQueue::NOT_IN_QUEUE) {

            // Get the popup
            YesNoPopupPageEx* popupPage = YesNoPopupPageEx::getPage();

            // Reset it and update the messages
            popupPage->reset();
            popupPage->setWindowMessage(Message::Menu::TRACK_UNPICKABLE_PROMPT_VS);
            popupPage->configureButton(0, Message::Menu::TRACK_UNPICKABLE_VOTE_ANYWAY, nullptr, Page::ANIM_NONE,
                                       (InputHandler2<Page, void, s32, PushButton*>*)&onRepickPromptPressHandler);
            popupPage->configureButton(1, Message::Menu::TRACK_UNPICKABLE_GO_BACK, nullptr, Page::ANIM_NONE,
                                       (InputHandler2<Page, void, s32, PushButton*>*)&onRepickPromptPressHandler);

            // Default to the Go Back button and allow going back
            popupPage->currSelected = 1;
            popupPage->isBackButtonEnabled = true;
            popupPage->onBackSelectedButton = 1;

            // Display the page
            repickPrompt = (YesNoPopupPageEx*)addPage(Page::ONLINE_VOTE_PROMPT, Page::ANIM_NEXT);

        // Else go to the next page
        } else {
            nextPageId = Page::NONE;
            replace(Page::ANIM_NEXT, button->getDelay());
        }

    } else {

        // Get the actual track to be played and store it
        u32 actualTrackIdx = CupManager::getTrackFile(trackIdx);
        CupManager::SetCourse(&RaceConfig::instance->menuScenario.settings, actualTrackIdx);

        // If we're in TT mode, go to the ghost select screen
        u32 gameMode = RaceConfig::instance->menuScenario.settings.gameMode;
        if (gameMode == RaceConfig::Settings::GAMEMODE_TT)
            loadNextPageById(Page::GHOST_SELECT_TOP, button);

        // If we're in VS mode, prepare intro and generate the track order from here
        else if (gameMode == RaceConfig::Settings::GAMEMODE_VS) {
            requestSectionChange(Section::DEMO_VS, button);
            CupManager::generateCourseOrder(cupIdx, button->buttonId);
        }
    }

    // Report course as selected
    courseSelected = true;
}

// Force press the selected option on the repick prompt when the timer runs out
void RaceCourseSelectPageEx::afterCalc() {

    // Check for active state
    if (pageState != Page::STATE_ACTIVE)
        return;

    // Check if we are online
    if (!UIUtils::isOnlineRoom(SectionManager::instance->curSection->sectionId))
        return;

    // Check if the timer is zero
    if (timer->value > 0.0f)
        return;

    // If the prompt is not enabled, vote random
    if (repickPrompt == nullptr) {
        forceRandomVote();

    // Else force press the selected button
    } else {
        repickPrompt->forcePressSelected();
    }
}

///////////////////////////
// Custom Engine Classes //
///////////////////////////

// Update instruction text with new CCs and Mirror option
void RaceCourseSelectPageEx::onActivate() {

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
