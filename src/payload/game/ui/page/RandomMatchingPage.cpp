#include <game/net/RKNetRH1Handler.hpp>
#include <game/system/RaceConfig.hpp>
#include <mvp/cup/CupManager.hpp>

///////////////////////////////////////////////
// Custom Cup System / Custom Engine Classes //
///////////////////////////////////////////////

// RandomMatchingPage::setupLiveView() patch
// Import race settings from RACEHEADER_1 packet (VS)
kmBranchDefCpp(0x80609EF0, 0x80609FA4, void) {
    RaceConfig::Settings* settings = &RaceConfig::instance->menuScenario.settings;

    // Store game mode and disable teams
    settings->gameMode = RaceConfig::Settings::GAMEMODE_PUBLIC_VS;
    settings->modeFlags &= ~RaceConfig::Settings::FLAG_TEAMS;

    // Store custom engine class data
    RKNetEngineClassData data(RKNetRH1Handler::instance->getEngineClass());
    settings->engineClass = data.getEngineClass();
    settings->modeFlags |= data.getIsMirrorFlag();

    // Store custom course data
    CupManager::SetCourse(settings, RKNetRH1Handler::instance->getTrackId());
}

// RandomMatchingPage::setupLiveView() patch
// Import race settings from RACEHEADER_1 packet (BT)
kmBranchDefCpp(0x80609FE0, 0x8060A038, void) {
    RaceConfig::Settings* settings = &RaceConfig::instance->menuScenario.settings;

    // Store game mode, battle type and enable teams
    settings->gameMode = RaceConfig::Settings::GAMEMODE_PUBLIC_BT;
    settings->battleType = RKNetRH1Handler::instance->getBattleType();
    settings->modeFlags |= RaceConfig::Settings::FLAG_TEAMS;

    // Store custom engine class data
    RKNetEngineClassData data(RKNetRH1Handler::instance->getEngineClass());
    settings->engineClass = RaceConfig::Settings::CC_50;
    settings->modeFlags |= data.getIsMirrorFlag();

    // Store custom course data
    CupManager::SetCourse(settings, RKNetRH1Handler::instance->getTrackId());
}
