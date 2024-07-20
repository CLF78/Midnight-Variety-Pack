#include <common/Common.hpp>
#include <game/ui/Section.hpp>
#include <mvp/cup/BattleCupSelectPageEx.hpp>
#include <mvp/cup/BattleStageSelectPageEx.hpp>
#include <mvp/cup/RaceCourseSelectPageEx.hpp>
#include <mvp/cup/RaceCupSelectPageEx.hpp>
#include <mvp/online/WifiMemberConfirmPageEx.hpp>
#include <mvp/online/WifiMenuPageEx.hpp>
#include <mvp/online/WifiModeSelectPageEx.hpp>
#include <mvp/online/YesNoPopupPageEx.hpp>
#include <mvp/transmission/TransmissionSelectPage.hpp>
#include <mvp/transmission/MultiTransmissionSelectPage.hpp>

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

        // Expand mode select to allow multiple tracklists
        case Page::WIFI_GAME_MODE_SELECT:
            return new WifiModeSelectPageEx();

        // Expand VR screen for various things
        case Page::VR_SCREEN:
            return new WifiMemberConfirmPageEx();

        // Inherits from DriftSelectPage: allows the player to select their transmission in SinglePlayer
        case Page::TRANSMISSION_SELECT:
            return new TransmissionSelectPage();

        // Inherits from MultiDriftSelectPage: allows the player to select their transmission in MultiPlayer
        case Page::TRANSMISSION_SELECT_MULTI_PLAYER:
            return new MultiTransmissionSelectPage();

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

        // Add transmission select to singleplayer sections
        case MENUSINGLE_FROM_MAIN:
        case MENUSINGLE_FROM_TT_CHANGE_CHAR:
        case MENUSINGLE_FROM_TT_CHANGE_COURSE:
        case MENUSINGLE_FROM_VS:
        case MENUSINGLE_FROM_BT:
        case MENUSINGLE_FROM_MR:
        case MENUSINGLE_FROM_CHANNEL_CHALLENGE_GHOST:
        case MENUSINGLE_FROM_LEADERBOARD_CHALLENGE_GHOST:
        case MENUSINGLE_FROM_GHOSTLIST_CHALLENGE_GHOST:
        case WIFI_MENU_1P:
        case WIFI_MENU_1P_FROM_FROOM:
        case WIFI_MENU_1P_FROM_FRIENDLIST:
            addPage(Page::TRANSMISSION_SELECT);
            REPLACED(sectionId);
            break;

        // Add transmission select to multiplayer sections
        case MENUMULTI:
        case WIFI_MENU_2P:
        case WIFI_MENU_2P_FROM_FROOM:
        case WIFI_MENU_2P_FROM_FRIENDLIST:
            addPage(Page::TRANSMISSION_SELECT_MULTI_PLAYER);
            REPLACED(sectionId);
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
