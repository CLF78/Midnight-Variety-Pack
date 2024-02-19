#include <common/Common.hpp>
#include <game/ui/ctrl/CtrlRaceBattleSetPoint.hpp>
#include <game/ui/ControlLoader.hpp>
#include <game/ui/UIUtils.hpp>
#include <game/ui/page/DemoPage.hpp>
#include <game/system/MultiDvdArchive.hpp>
#include <game/system/ResourceManager.hpp>
#include <platform/stdio.h>
#include <midnight/cup/CupManager.hpp>

///////////////////////
// Custom Cup System //
///////////////////////

// DemoPage::onInit() override
// Apply custom cup icons and cup/track names
kmPointerDefCpp(0x808DA590, void, DemoPage* self) {

    // Get current section
    u32 curSection = SectionManager::instance->curSection->sectionID;
    RaceConfig::Settings* raceSettings = &RaceConfig::instance->raceScenario.settings;
    GlobalContext* globalCtx = SectionManager::instance->globalContext;

    // Initialize input manager and set it
    self->inputManager.init(0, false);
    self->setInputManager(&self->inputManager);

    // Initialize child count (add the bubbles if we're in battle mode)
    u32 childCount = 2;
    u32 childIdx = 0;
    if (curSection == Section::DEMO_BT)
        childCount += globalCtx->raceCount * 2;
    self->initChildren(childCount);

    // Initialize course name display
    self->insertChild(childIdx++, &self->courseName, 0);
    ControlLoader courseDisplayLoader(&self->courseName);
    courseDisplayLoader.load("demo", "course_name", "course_name", nullptr);

    // Set the course name
    u16 trackName = CupManager::getTrackName(CupManager::currentSzs);
    self->courseName.setText(trackName, nullptr);

    // Initialize the top text
    self->insertChild(childIdx++, &self->topText, 0);
    ControlLoader topTextLoader(&self->topText);
    topTextLoader.load("demo", "cup_name", "cup_name", nullptr);

    // Set everything else depending on the mode
    MessageInfo msgInfo;

    if (curSection == Section::DEMO_GP) {

        // Replace the cup icon
        u32 cupIdx = raceSettings->cupId;
        const char* cupIcon = CupManager::replaceCupIcon(&self->topText, cupIdx);
        self->topText.setMatIcon("cup_icon", cupIcon);

        // Set the cup name in the message info
        msgInfo.messageIds[0] = CupManager::GetCupList()[cupIdx].cupName;

        // Set the text depending on the race number
        self->topText.setText(1410 + raceSettings->raceNumber, &msgInfo);
    
    } else if (curSection == Section::DEMO_VS) {

        // Set the VS flag as the icon
        self->topText.setMatIcon("cup_icon", "icon_11_flag");

        // Set the CC argument
        switch (raceSettings->engineClass) {
            case RaceConfig::Settings::CC_50:
                msgInfo.messageIds[0] = 1417;
                break;

            case RaceConfig::Settings::CC_100:
                msgInfo.messageIds[0] = 1418;
                break;

            case RaceConfig::Settings::CC_150:
                msgInfo.messageIds[0] = 1419;
                break;

            case RaceConfig::Settings::CC_200:
                msgInfo.messageIds[0] = 1600;
                break;

            case RaceConfig::Settings::CC_500:
                msgInfo.messageIds[0] = 1601;
                break;
        }

        // Set the mirror argument
        if (raceSettings->modeFlags & RaceConfig::Settings::FLAG_MIRROR) {
            msgInfo.messageIds[1] = 1420;
            msgInfo.strings[0] = L" ";
        }

        // Update the race number and insert it into the message together with the race count
        msgInfo.intVals[0] = ++globalCtx->currentRace;
        msgInfo.intVals[1] = globalCtx->raceCount;

        // Set the message number
        u32 msgId = (raceSettings->modeFlags & RaceConfig::Settings::FLAG_TEAMS) ? 1409 : 1414;
        self->topText.setText(msgId, &msgInfo);
    
    } else if (curSection == Section::DEMO_BT) {

        // Update race number
        globalCtx->currentRace++;

        // Set icon and top text depending on the battle mode
        if (raceSettings->battleType == RaceConfig::Settings::BATTLE_BALLOON) {
            self->topText.setMatIcon("cup_icon", "icon_09_balloon");
            self->topText.setText(1415, nullptr);
        } else {
            self->topText.setMatIcon("cup_icon", "icon_10_coin");
            self->topText.setText(1416, nullptr);
        }

        // Create red bubbles
        for (int i = 0; i < globalCtx->raceCount; i++) {
            CtrlRaceBattleSetPoint* redBubble = new CtrlRaceBattleSetPoint();
            self->insertChild(childIdx++, redBubble, 0);
            redBubble->load(false, i);
        }

        // Create blue bubbles
        for (int i = 0; i < globalCtx->raceCount; i++) {
            CtrlRaceBattleSetPoint* blueBubble = new CtrlRaceBattleSetPoint();
            self->insertChild(childIdx++, blueBubble, 0);
            blueBubble->load(true, i);
        }
    
    // On other modes, hide the top text altogether
    } else {
        self->topText.hidden = true;
    }
}
