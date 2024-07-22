#include <common/Common.hpp>
#include <game/ui/ControlLoader.hpp>
#include <game/ui/Message.hpp>
#include <game/ui/UIUtils.hpp>
#include <game/ui/ctrl/CtrlRaceBattleSetPoint.hpp>
#include <game/ui/page/DemoPage.hpp>
#include <game/system/MultiDvdArchive.hpp>
#include <game/system/ResourceManager.hpp>
#include <platform/stdio.h>
#include <mvp/cup/CupManager.hpp>

///////////////////////////////////////////////
// Custom Cup System / Custom Engine Classes //
///////////////////////////////////////////////

// Apply custom cup icons and cup/track names
REPLACE void DemoPage::onInit() {

    // Get current section
    u32 curSection = SectionManager::instance->curSection->sectionId;
    RaceConfig::Settings* raceSettings = &RaceConfig::instance->raceScenario.settings;
    GlobalContext* globalCtx = SectionManager::instance->globalContext;

    // Initialize input manager and set it
    inputManager.init(0, false);
    setInputManager(&inputManager);

    // Initialize child count (add the bubbles if we're in battle mode)
    u32 childCount = 2;
    u32 childIdx = 0;
    if (curSection == Section::DEMO_BT)
        childCount += globalCtx->raceCount * 2;
    initChildren(childCount);

    // Initialize course name display
    insertChild(childIdx++, &courseName, 0);
    ControlLoader courseDisplayLoader(&courseName);
    courseDisplayLoader.load("demo", "course_name", "course_name", nullptr);

    // Set the course name
    u16 trackName = CupManager::getTrackName(CupManager::currentSzs);
    courseName.setText(trackName);

    // Initialize the top text
    insertChild(childIdx++, &topText, 0);
    ControlLoader topTextLoader(&topText);
    topTextLoader.load("demo", "cup_name", "cup_name", nullptr);

    // Set everything else depending on the mode
    MessageInfo msgInfo;

    if (curSection == Section::DEMO_GP) {

        // Replace the cup icon
        u32 cupIdx = raceSettings->cupId;
        const char* cupIcon = CupManager::replaceCupIcon(&topText, cupIdx);
        topText.setMatIcon("cup_icon", cupIcon);

        // Set the cup name in the message info
        msgInfo.messageIds[0] = CupManager::GetCupList()[cupIdx].cupName;

        // Set the text depending on the race number
        topText.setText(Message::Race::GP_RACE_1 + raceSettings->raceNumber, &msgInfo);
    
    } else if (curSection == Section::DEMO_VS) {

        // Set the VS flag as the icon
        topText.setMatIcon("cup_icon", "icon_11_flag");

        // Set the CC argument
        switch (raceSettings->engineClass) {
            case RaceConfig::Settings::CC_50:
                msgInfo.messageIds[0] = Message::Race::CC_50;
                break;

            case RaceConfig::Settings::CC_100:
                msgInfo.messageIds[0] = Message::Race::CC_100;
                break;

            case RaceConfig::Settings::CC_150:
                msgInfo.messageIds[0] = Message::Race::CC_150;
                break;

            case RaceConfig::Settings::CC_200:
                msgInfo.messageIds[0] = Message::Race::CC_200;
                break;

            case RaceConfig::Settings::CC_500:
                msgInfo.messageIds[0] = Message::Race::CC_500;
                break;
        }

        // Update the race number and insert it into the message together with race count and mirror word
        msgInfo.setCondMessageValue(2, raceSettings->modeFlags & RaceConfig::Settings::FLAG_MIRROR != 0, true);
        msgInfo.intVals[0] = ++globalCtx->currentRace;
        msgInfo.intVals[1] = globalCtx->raceCount;

        // Set the message number
        u32 msgId = (raceSettings->modeFlags & RaceConfig::Settings::FLAG_TEAMS) ?
                    Message::Race::TEAM_VS :
                    Message::Race::VS;

        topText.setText(msgId, &msgInfo);
    
    } else if (curSection == Section::DEMO_BT) {

        // Update race number
        globalCtx->currentRace++;

        // Set icon and top text depending on the battle mode
        if (raceSettings->battleType == RaceConfig::Settings::BATTLE_BALLOON) {
            topText.setMatIcon("cup_icon", "icon_09_balloon");
            topText.setText(Message::Race::BALLOON_BATTLE);
        } else {
            topText.setMatIcon("cup_icon", "icon_10_coin");
            topText.setText(Message::Race::COIN_RUNNERS);
        }

        // Create red bubbles
        for (int i = 0; i < globalCtx->raceCount; i++) {
            CtrlRaceBattleSetPoint* redBubble = new CtrlRaceBattleSetPoint();
            insertChild(childIdx++, redBubble, 0);
            redBubble->load(false, i);
        }

        // Create blue bubbles
        for (int i = 0; i < globalCtx->raceCount; i++) {
            CtrlRaceBattleSetPoint* blueBubble = new CtrlRaceBattleSetPoint();
            insertChild(childIdx++, blueBubble, 0);
            blueBubble->load(true, i);
        }
    
    // On other modes, hide the top text altogether
    } else {
        topText.hidden = true;
    }
}
