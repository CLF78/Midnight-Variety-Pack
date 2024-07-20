#include <common/Common.hpp>
#include <game/sound/AudioHandleHolder.hpp>
#include <game/system/RaceConfig.hpp>
#include <game/ui/SectionManager.hpp>
#include <game/ui/page/SinglePlayerMenuPage.hpp>
#include <mvp/cup/CupManager.hpp>

///////////////////////////////////////////////
// Custom Cup System / Custom Engine Classes //
///////////////////////////////////////////////

// Update settings based on the selected button
REPLACE void SinglePlayerMenuPage::onButtonClick(PushButton* button, u32 hudSlotId) {

    // Game mode for each menu button
    static const u32 gameModes[SinglePlayerMenuPage::BUTTON_COUNT] = {
        RaceConfig::Settings::GAMEMODE_GP,
        RaceConfig::Settings::GAMEMODE_TT,
        RaceConfig::Settings::GAMEMODE_VS,
        RaceConfig::Settings::GAMEMODE_BT,
        RaceConfig::Settings::GAMEMODE_MR
    };

    // Camera mode for each menu button
    static const u32 cameraModes[SinglePlayerMenuPage::BUTTON_COUNT] = {
        RaceConfig::Settings::CAMERA_MODE_GAMEPLAY_INTRO,
        RaceConfig::Settings::CAMERA_MODE_GAMEPLAY_NO_INTRO,
        RaceConfig::Settings::CAMERA_MODE_GAMEPLAY_INTRO,
        RaceConfig::Settings::CAMERA_MODE_GAMEPLAY_INTRO,
        RaceConfig::Settings::CAMERA_MODE_GAMEPLAY_NO_INTRO
    };

    // Player count for each menu button
    static const u8 playerCounts[SinglePlayerMenuPage::BUTTON_COUNT] = { 12, 1, 12, 12, 1 };

    // Next screen for each menu button
    static const u32 nextPages[SinglePlayerMenuPage::BUTTON_COUNT] = {
        Page::CHARACTER_SELECT,
        Page::GP_CLASS_SELECT,
        Page::VS_MODE_PROMPT,
        Page::BT_MODE_PROMPT,
        Page::LEVEL_SELECT_MR
    };

    // Get menu scenario
    RaceConfig::Scenario* scenario = &RaceConfig::instance->menuScenario;

    // Reset vehicle restriction
    SectionManager::instance->globalContext->vehicleRestriction = GlobalContext::VEHICLES_ALL;

    // Reset mode flags and item setting
    scenario->settings.modeFlags = 0;
    scenario->settings.itemMode = RaceConfig::Settings::ITEMS_BALANCED;

    // Get the selected button's ID
    u32 buttonId = button->buttonId;

    // If the back button was pressed, return to the main menu
    if (buttonId == MenuPage::BACK_BUTTON) {
        forceSectionChange(Section::MENU_FROM_GAME, button);
        u32 soundId = Section::getSoundID(Section::MENU_FROM_GAME);
        AudioHandleHolder::instance->prepare(soundId, false);
        return;
    }

    // Set various properties
    switch (buttonId) {

        // Force 150cc, Hard CPUs and the configured GP tracklist in GP mode
        case SinglePlayerMenuPage::GP_BUTTON:
            scenario->settings.engineClass = RaceConfig::Settings::CC_150;
            scenario->settings.cpuMode = RaceConfig::Settings::CPU_HARD;
            CupManager::currentCupList = CupManager::TRACKS_GP;
            break;

        // Force the configured TT tracklist in TT mode
        // Ignore CC setting, as it will be overridden by the CC selector screen
        case SinglePlayerMenuPage::TT_BUTTON:
            CupManager::currentCupList = CupManager::TRACKS_TT;
            break;

        // Force the default VS tracklist in VS mode
        // TODO set proper tracklist for VS mode
        case SinglePlayerMenuPage::VS_BUTTON:
            CupManager::currentCupList = CupManager::TRACKS_VS_DEFAULT;
            break;

        // Force 50cc and the default battle tracklist in Battle mode
        // TODO set proper tracklist for BT mode
        case SinglePlayerMenuPage::BT_BUTTON:
            scenario->settings.engineClass = RaceConfig::Settings::CC_50;
            CupManager::currentBattleCupList = CupManager::TRACKS_BT_DEFAULT;

        // Do nothing for Mission mode
        default:
            break;
    }

    // Set the correct game mode and camera mode
    scenario->settings.gameMode = gameModes[buttonId];
    scenario->settings.cameraMode = cameraModes[buttonId];

    // Set the appropriate player types based on total player count
    u8 playerCount = playerCounts[buttonId];
    for (int i = 0; i < playerCount; i++) {
        scenario->players[i].playerType = (i == 0) ? RaceConfig::Player::TYPE_LOCAL
                                                   : RaceConfig::Player::TYPE_CPU;
    }

    // Set all excluded players to NONE
    if (playerCount < 12) {
        for (int i = playerCount; i < 12; i++) {
            scenario->players[i].playerType = RaceConfig::Player::TYPE_NONE;
        }
    }

    // Go to the next screen
    loadNextPageById(nextPages[buttonId], button);
}
