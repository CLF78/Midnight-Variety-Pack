#include "AwardPage.hpp"
#include <game/system/RaceConfig.hpp>
#include <game/system/SaveManager.hpp>
#include <game/ui/Message.hpp>
#include <game/ui/SectionManager.hpp>
#include <mvp/cup/CupManager.hpp>
#include <mvp/save/SaveExpansionCup.hpp>

///////////////////////
// Custom Cup System //
///////////////////////

// Trigger the credits if all GPs have been completed
// TODO figure out which TU this code belongs to and move it there
REPLACE_STATIC AwardPage::CreditsType AwardPage::getCreditsType(u32 cupId, u32 engineClass,
                                                                bool isMirror, u32 rank) {

    // Copied check from the original function
    if (rank == 0 || rank > 3)
        return AwardPage::CREDITS_NONE;

    // Get current license, bail if invalid
    SaveManager* save = SaveManager::instance;
    if (save->currentLicenseId == -1)
        return AwardPage::CREDITS_NONE;

    // Check that each cup is completed
    SaveExpansionCup* cups = SaveExpansionCup::GetSection();
    for (u32 i = 0; i < SaveExpansionCup::GetEntryCount(); i++) {
        if (!cups->GetData(i)->mCompleted)
            return AwardPage::CREDITS_NONE;
    }

    // If so go to the real credits
    return AwardPage::CREDITS_FULL;
}

// Save the GP rank to the save expansion
REPLACE void AwardPage::initRank() {

    // Check if we're in GP mode
    if (SectionManager::instance->curSection->sectionId != Section::AWARD_GP)
        return;

    // Obtain GP rank
    RaceConfig* rconfig = RaceConfig::instance;
    const u32 gpRank = rconfig->awardsScenario.players[0].computeGPRank();

    // Get current license, bail if invalid
    SaveManager* save = SaveManager::instance;
    if (save->currentLicenseId == -1)
        return;

    // Check if the new rank is better than the existing one
    const u32 cupId = rconfig->awardsScenario.settings.cupId;
    SaveExpansionCup::Data* cupData = SaveExpansionCup::GetSection()->GetData(cupId);
    if (cupData->mRank <= gpRank)
        return;

    // Store the data
    cupData->mCompleted = true;
    cupData->mRank = gpRank;

    // Mark license as dirty
    SectionManager::instance->saveGhostManager->markLicensesDirty();
}

// AwardPage::initCup() patch
// Use the VS trophy for the GP award scene
kmWrite8(0x805BCF87, 0x90);

///////////////////////////////////////////////
// Custom Cup System / Custom Engine Classes //
///////////////////////////////////////////////

// Set the proper icons and text messages for each mode
REPLACE void AwardPage::initType() {

    // Initialize values
    u32 msgId = Message::NONE;
    MessageInfo msgInfo;
    const char* iconPane = nullptr;

    // Get relevant information
    const u32 curSection = SectionManager::instance->curSection->sectionId;
    RaceConfig::Settings* settings = &RaceConfig::instance->awardsScenario.settings;

    // Set icon and message for battle mode awards
    if (curSection == Section::AWARD_BT) {
        const u32 battleType = settings->battleType;
        if (battleType == RaceConfig::Settings::BATTLE_BALLOON) {
            msgId = Message::Race::BALLOON_BATTLE;
            iconPane = "icon_09_balloon";
        }
        else if (battleType == RaceConfig::Settings::BATTLE_COIN) {
            msgId = Message::Race::COIN_RUNNERS;
            iconPane = "icon_10_coin";
        }

    // Set icon and message for VS and GP modes
    } else if (curSection == Section::AWARD_GP || curSection == Section::AWARD_VS_36 || curSection == Section::AWARD_VS_37) {

        // Get the cup icon (or the flag icon if not in GP mode)
        if (curSection == Section::AWARD_GP)
            iconPane = CupManager::replaceCupIcon(&cupDisplay, settings->cupId);
        else
            iconPane = "icon_11_flag";

        // Get the message depending on the mode
        msgId = (curSection == Section::AWARD_GP) ?
                Message::Race::AWARD_GP :
                Message::Race::AWARD_VS;

        // Set the CC
        const u32 engineClass = settings->engineClass;
        switch (engineClass) {
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

        // Set the cup name/mirror
        if (curSection == Section::AWARD_GP)
            msgInfo.messageIds[0] = CupManager::GetCup(settings->cupId)->cupName;
        else
            msgInfo.setCondMessageValue(0, (settings->modeFlags & RaceConfig::Settings::FLAG_MIRROR) != 0, true);
    }

    // Apply changes
    cupDisplay.setMatIcon("cup_icon", iconPane);
    cupDisplay.setMatText("cup_name", msgId, &msgInfo);
}
