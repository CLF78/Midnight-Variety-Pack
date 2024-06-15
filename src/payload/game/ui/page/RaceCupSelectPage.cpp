#include <common/Common.hpp>
#include <game/system/RaceConfig.hpp>
#include <game/system/SaveManager.hpp>
#include <game/ui/page/RaceCupSelectPage.hpp>
#include <game/ui/Message.hpp>
#include <game/ui/SectionManager.hpp>
#include <game/ui/UIUtils.hpp>
#include <midnight/cup/CupManager.hpp>
#include <midnight/save/SaveExpansionCup.hpp>

///////////////////////
// Custom Cup System //
///////////////////////

// Construct the expansion data
RaceCupSelectPageEx::RaceCupSelectPageEx() :
    arrows(),
    leftHandler((RaceCupSelectArrow*)&arrows.leftButton, &RaceCupSelectArrow::onLeftArrowPress),
    rightHandler((RaceCupSelectArrow*)&arrows.rightButton, &RaceCupSelectArrow::onRightArrowPress),
    curPage(CupManager::getCupPageFromTrack(SectionManager::instance->globalContext->lastCourse)) {
        arrows.leftHandler = (typeof(arrows.leftHandler))&leftHandler;
        arrows.rightHandler = (typeof(arrows.rightHandler))&rightHandler;
        layoutCount++;
}

UIControl* RaceCupSelectPageEx::loadLayout(u32 layoutIdx) {
    switch (layoutIdx) {

        // Load the cup holder
        case LYT_CUP_SELECT_CUP:
            insertChild(curChildCount++, &cupHolder, 0);
            cupHolder.load(1, false);
            return &cupHolder;

        // Load the course holder
        case LYT_CUP_SELECT_COURSE:
            insertChild(curChildCount++, &courseHolder, 0);
            courseHolder.load();
            return &courseHolder;

        // Load the cup arrows
        case LYT_CUP_ARROWS:
            insertChild(curChildCount++, &arrows, 0);

            // Determine the variant to use depending on the player count
            const char* rightVar = UIUtils::getPlayerCount() > 2 ? "ButtonArrowRight2" : "ButtonArrowRight";
            const char* leftVar = UIUtils::getPlayerCount() > 2 ? "ButtonArrowLeft2" : "ButtonArrowLeft";

            // Load BRCTR
            arrows.load("button", CUP_ARROW_R_BRCTR, rightVar, CUP_ARROW_L_BRCTR, leftVar, 1, false, false);
            return &arrows;

        // Invalid child
        default:
            return nullptr;
    }
}

// Display the GP rank from the expanded save
// Original function address: 80841614
void RaceCupSelectPageEx::updateTextMessages(CtrlMenuCupSelectCup* cupHolder, u32 unk) {

    // Update course names
    u32 cupButtonId = cupHolder->currentSelected;
    courseHolder.setCourseNames(cupButtonId);

    // If game mode is Grand Prix, update the message at the bottom with the cup rank
    if (RaceConfig::instance->menuScenario.settings.gameMode == RaceConfig::Settings::GAMEMODE_GP) {

        // Initialize info
        MessageInfo msgInfo;

        // Get current license, bail if invalid
        SaveManager* save = SaveManager::instance;
        if (save->currentLicenseId == -1)
            return;

        // Get the cup entry
        u32 cupId = CupManager::getCupIdxFromButton(cupButtonId, curPage);
        SaveExpansionCup::Data* cupData = SaveExpansionCup::GetSection()->GetData(cupId);

        // Get the message ID
        if (cupData->mCompleted)
            msgInfo.messageIds[0] = Message::Menu::GP_RANK_3_STARS + cupData->mRank;
        else
            msgInfo.messageIds[0] = Message::Menu::GP_RANK_NONE;

        // Set it
        this->instructionText->setText(Message::Menu::INSTRUCTION_TEXT_GP_RANK, &msgInfo);
    }
}

// Set the selected track when a cup is clicked
void RaceCupSelectPageEx::setCourse(CtrlMenuCupSelectCup* cupHolder, PushButton* button) {

    // Check for defocusing state
    if (pageState == Page::STATE_DEFOCUSING) {

        // Update selected button
        selectedButtonId = cupHolder->currentSelected;

        // Get the cup and its first track
        u32 cupIdx = CupManager::getCupIdxFromButton(selectedButtonId, curPage);
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
            CupManager::SetCourse(&RaceConfig::instance->menuScenario.settings, actualTrackIdx);

            // If in GP mode, go straight to the OK button instead of the course selection
            if (RaceConfig::instance->menuScenario.settings.gameMode == RaceConfig::Settings::GAMEMODE_GP) {
                requestSectionChange(Section::DEMO_GP, button);
                return;
            }

        // Else wait for the course voting page to be loaded (is this even needed?)
        } else
            while (SectionManager::instance->curSection->pages[Page::WIFI_VOTING] == nullptr) {}

        // Go to the course select page
        startReplace(Page::COURSE_SELECT, button);
    }
}

// RaceCupSelectPage::onRefocus() patch
// Disable background movies
kmBranch(0x80841F74, 0x80841FD4);

///////////////////////////////////////////////
// Custom Cup System / Custom Engine Classes //
///////////////////////////////////////////////

// A lot of stuff (see code comments)
void RaceCupSelectPageEx::onActivate() {

    // Set top message
    if (RaceConfig::instance->menuScenario.settings.gameMode == RaceConfig::Settings::GAMEMODE_GP)
        titleBmgId = Message::Menu::SELECT_CUP;
    else
        titleBmgId = Message::Menu::SELECT_COURSE;

    // Set default cup button to the cup the previously selected track belongs to
    if (animId == Page::ANIM_NEXT) {
        u32 lastTrack = SectionManager::instance->globalContext->lastCourse;
        selectedButtonId = CupManager::getCupButtonFromTrack(lastTrack, curPage);
    }

    // Call base function
    MenuPage::onActivate();

    // Adjust X wrapping and arrow display by setting the distance function appropriately
    // 0 wraps on the X and Y axis, 1 wraps on Y axis only
    bool arrowsEnabled = CupManager::GetCupArrowsEnabled();
    int wrapType = (CupManager::GetCupCount() == 2 || arrowsEnabled);
    multiControlInputManager.setDistanceFunc(wrapType);

    // Disable the arrows if not required
    arrows.configure(arrowsEnabled, arrowsEnabled);

    // Initialize cup holder
    cupHolder.init();

    // Load the Vote/Random popup if we are in an online room
    if (UIUtils::isOnlineRoom(SectionManager::instance->curSection->sectionID) && animId == Page::ANIM_NEXT) {

        // Get the page
        YesNoPopupPage* popupPage = YesNoPopupPage::getPage();

        // Reset it and update the messages
        popupPage->reset();
        popupPage->setWindowMessage(Message::Menu::VOTE_FOR_COURSE_QUESTION, nullptr);
        popupPage->configureButton(0, Message::Menu::VOTE, nullptr, Page::ANIM_NONE, nullptr);
        popupPage->configureButton(1, Message::Menu::VOTE_RANDOM, nullptr, Page::ANIM_NONE, nullptr);

        // Default to the Vote button
        popupPage->currSelected = 0;

        // Display the page and store a reference to it
        popupPage = (YesNoPopupPage*)addPage(Page::ONLINE_VOTE_PROMPT, Page::ANIM_NEXT);
        voteOrRandomPage = popupPage;
    }

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
