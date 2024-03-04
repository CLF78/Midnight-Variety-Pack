#include <common/Common.hpp>
#include <game/system/RaceConfig.hpp>
#include <game/system/SaveManager.hpp>
#include <game/ui/page/AwardPage.hpp>
#include <game/ui/SectionManager.hpp>
#include <midnight/cup/CupManager.hpp>

///////////////////////
// Custom Cup System //
///////////////////////

// Trigger the credits if all GPs have been completed
// TODO figure out which TU this code belongs to and move it there
kmCallDefCpp(0x805BC788, int, u32 cupId, u32 engineClass, bool isMirror, u32 rank) {

    // Copied check from the original function
    if (rank == 0 || rank > 3)
        return AwardPage::CREDITS_NONE;

    // Get current license, bail if invalid
    SaveManager* save = SaveManager::instance;
    if (save->currentLicenseId == -1)
        return AwardPage::CREDITS_NONE;

    // Check that each cup is completed
    SaveExpansion::Cup* cups = save->expansion.licensesEx[save->currentLicenseId].gpRanks;
    for (int i = 0; i < CupManager::GetCupListData(CupManager::TRACKS_MODERN)->cupCount; i++) {
        if (!cups[i].completed)
            return AwardPage::CREDITS_NONE;
    }

    // If so go to the real credits
    return AwardPage::CREDITS_FULL;
}

// AwardPage::initCup() patch
// Use the VS trophy for the GP award scene
kmWrite8(0x805BCF87, 0x90);

// AwardPage::initRank() override
// Save the GP rank to the save expansion
kmBranchDefCpp(0x805BD050, NULL, void) {

    // Check if we're in GP mode
    if (SectionManager::instance->curSection->sectionID != Section::AWARD_GP)
        return;

    // Obtain GP rank
    RaceConfig* rconfig = RaceConfig::instance;
    int gpRank = rconfig->awardsScenario.players[0].computeGPRank();

    // Get current license, bail if invalid
    SaveManager* save = SaveManager::instance;
    if (save->currentLicenseId == -1)
        return;

    // Get the cup entry
    SaveExpansion* licenseEx = &save->expansion.licensesEx[save->currentLicenseId];
    u32 cupId = rconfig->awardsScenario.settings.cupId;

    // Check if the new rank is better than the existing one
    SaveExpansion::Cup* cup = &licenseEx->gpRanks[cupId];
    if (cup->rank <= gpRank)
        return;

    // Store the data
    cup->completed = true;
    cup->rank = gpRank;

    // Mark license as dirty
    SectionManager::instance->saveGhostManager->markLicensesDirty();
}

///////////////////////////////////////////////
// Custom Cup System / Custom Engine Classes //
///////////////////////////////////////////////

// AwardPage::initType() override
// Set the proper icons and text messages for each mode
kmBranchDefCpp(0x805BC9DC, NULL, void, AwardPage* self) {

    // Initialize values
    u32 msgId = 0;
    MessageInfo msgInfo;
    const char* iconPane = nullptr;

    // Get relevant information
    u32 curSection = SectionManager::instance->curSection->sectionID;
    RaceConfig::Settings* settings = &RaceConfig::instance->awardsScenario.settings;

    // Set icon and message for battle mode awards
    if (curSection == Section::AWARD_BT) {
        u32 battleType = settings->battleType;
        if (battleType == RaceConfig::Settings::BATTLE_BALLOON) {
            msgId = 1415;
            iconPane = "icon_09_balloon";
        }
        else if (battleType == RaceConfig::Settings::BATTLE_COIN) {
            msgId = 1416;
            iconPane = "icon_10_coin";
        }

    // Set icon and message for VS and GP modes
    } else if (curSection == Section::AWARD_GP || curSection == Section::AWARD_VS_36 || curSection == Section::AWARD_VS_37) {

        // Get the cup icon (or the flag icon if not in GP mode)
        if (curSection == Section::AWARD_GP)
            iconPane = CupManager::replaceCupIcon(&self->cupDisplay, settings->cupId);
        else
            iconPane = "icon_11_flag";

        // Get the message depending on the mode
        msgId = (curSection == Section::AWARD_GP) ? 1421 : 1422;

        // Set the CC
        u32 engineClass = settings->engineClass;
        switch (engineClass) {
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

        // Set the cup name/mirror
        if (curSection == Section::AWARD_GP)
            msgInfo.messageIds[0] = CupManager::GetCupList()[settings->cupId].cupName;
        else
            msgInfo.setCondMessageValue(0, settings->modeFlags & RaceConfig::Settings::FLAG_MIRROR != 0, true);
    }

    // Apply changes
    self->cupDisplay.setMatIcon("cup_icon", iconPane);
    self->cupDisplay.setMatText("cup_name", msgId, &msgInfo);
}
