#include <common/Common.hpp>
#include <game/ui/UIUtils.hpp>

namespace UIUtils {

RaceConfig::Scenario* getScenario() {

    // Failsaves
    RaceConfig* instance = RaceConfig::instance;
    if (instance == nullptr)
        return nullptr;

    Section* section = SectionManager::instance->curSection;
    if (section == nullptr)
        return &instance->menuScenario;

    // Get the proper scenario for the current section
    switch (section->sectionID) {

        // The award scenario is only used in the award scenes
        case Section::AWARD_GP ... Section::AWARD_BT:
            return &instance->awardsScenario;

        // The race scenario is used in much of the game
        case Section::DEMO_GP ... Section::GHOST_REPLAY_FROM_TT:
        case Section::MENU_DEMO_GAMEPLAY:
        case Section::RACE_WIFI_VS_1P ... Section::RACE_FROOM_COIN_BATTLE_2P:
            return &instance->raceScenario;

        // Fall back to the menu scenario otherwise
        default:
            return &instance->menuScenario;
    }
}

} // namespace UIUtils
