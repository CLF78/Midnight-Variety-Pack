#include <common/Common.hpp>
#include <game/ui/Section.hpp>
#include <midnight/cup/BattleCupSelectPageEx.hpp>
#include <midnight/cup/BattleStageSelectPageEx.hpp>
#include <midnight/cup/RaceCourseSelectPageEx.hpp>
#include <midnight/cup/RaceCupSelectPageEx.hpp>
#include <midnight/online/WifiMemberConfirmPageEx.hpp>
#include <midnight/online/WifiMenuPageEx.hpp>
#include <midnight/online/YesNoPopupPageEx.hpp>

///////////////////////////
// Page Expansion System //
///////////////////////////

// Apply page replacements
REPLACE_STATIC Page* Section::createPage(Page::PageID pageId) {
    switch (pageId) {

        // Expand yes/no prompt to allow going back
        case Page::ONLINE_VOTE_PROMPT:
            return new YesNoPopupPageEx();

        // Expand cup select for custom cup system
        case Page::CUP_SELECT:
            return new RaceCupSelectPageEx();

        // Expand course select for custom cup system
        case Page::COURSE_SELECT:
            return new RaceCourseSelectPageEx();

        // Expand battle cup select for custom cup system
        case Page::CUP_SELECT_BT:
            return new BattleCupSelectPageEx();

        // Expand battle course select for custom cup system
        case Page::COURSE_SELECT_BT:
            return new BattleStageSelectPageEx();

        // Expand main online menu for Wiimmfi console assignment message
        case Page::WIFI_MENU:
            return new WifiMenuPageEx();

        // Expand VR screen for various things
        case Page::VR_SCREEN:
            return new WifiMemberConfirmPageEx();

        // Fallback
        default:
            return REPLACED(pageId);
    }
}

// Apply section page replacements (all pages)
REPLACE void Section::addPages(SectionID sectionId) {

    switch(sectionId) {

        // Replace save error page to prevent using the game without saving
        case SAVE_CANNOT_FLUSH:
            REPLACED(SAVE_CANNOT_READ_RFL);
            break;

        // Add the message popup to display the rules
        case WIFI_VS_1P_VOTE:
        case WIFI_BT_1P_VOTE:
        case WIFI_VS_2P_VOTE:
        case WIFI_BT_2P_VOTE:
        case WIFI_FROOM_1P_VS_VOTE:
        case WIFI_FROOM_1P_TEAM_VS_VOTE:
        case WIFI_FROOM_1P_BALLOON_BATTLE_VOTE:
        case WIFI_FROOM_1P_COIN_RUNNERS_VOTE:
        case WIFI_FROOM_2P_VS_VOTE:
        case WIFI_FROOM_2P_TEAM_VS_VOTE:
        case WIFI_FROOM_2P_BALLOON_BATTLE_VOTE:
        case WIFI_FROOM_2P_COIN_RUNNERS_VOTE:
            addPage(Page::MESSAGE_POPUP);
            REPLACED(sectionId);
            break;

        // Fallback
        default:
            REPLACED(sectionId);
            break;
    }
}

// Apply section page replacements (starting pages)
REPLACE void Section::addActivePages(SectionID sectionId) {

    switch(sectionId) {

        // Replace save error page to prevent using the game without saving
        case SAVE_CANNOT_FLUSH:
            REPLACED(SAVE_CANNOT_READ_RFL);
            break;

        // Fallback
        default:
            REPLACED(sectionId);
            break;
    }
}
