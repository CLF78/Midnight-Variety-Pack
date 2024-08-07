#include "Section.hpp"
#include <mvp/cup/BattleCupSelectPageEx.hpp>
#include <mvp/cup/BattleStageSelectPageEx.hpp>
#include <mvp/cup/RaceCourseSelectPageEx.hpp>
#include <mvp/cup/RaceCupSelectPageEx.hpp>
#include <mvp/online/WifiMemberConfirmPageEx.hpp>
#include <mvp/online/WifiMenuPageEx.hpp>
#include <mvp/online/WifiModeSelectPageEx.hpp>
#include <mvp/online/YesNoPopupPageEx.hpp>
#include <mvp/transmission/MultiTransmissionSelectPage.hpp>
#include <mvp/transmission/TransmissionSelectPage.hpp>

///////////////////////////
// Page Expansion System //
///////////////////////////

// Apply page replacements
REPLACE_STATIC Page* Section::createPage(Page::PageId pageId) {
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

        // Allow the player to select their transmission in single player
        case Page::TRANSMISSION_SELECT:
            return new TransmissionSelectPage();

        // Allow the player to select their transmission in multiplayer
        case Page::TRANSMISSION_SELECT_MULTI_PLAYER:
            return new MultiTransmissionSelectPage();

        // Fallback
        default:
            return REPLACED(pageId);
    }
}

// Apply section page replacements (all pages)
REPLACE void Section::addPages(Section::SectionId sectionId) {

    switch(sectionId) {

        // Replace save error page to prevent using the game without saving
        case SAVE_CANNOT_FLUSH:
            REPLACED(SAVE_CANNOT_READ_RFL);
            break;

        // Add transmission select to single player sections
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

// Apply section page replacements (initial pages)
REPLACE void Section::addActivePages(Section::SectionId sectionId) {

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

// Allow adding custom pages
REPLACE void Section::addPage(Page::PageId pageId) {
    Page* page = createPage(pageId);
    setPage(pageId, page);
    page->init(pageId);
}

// Allow adding custom initial pages
REPLACE void Section::addActivePage(Page::PageId pageId) {
    Page::AnimationDirection direction = animationDirection;
    Page* page = getPage(pageId);
    activePages[activePageCount++] = page;

    page->setAnimationDirection(direction);
    page->activate();
}

// Allow activating custom pages
REPLACE Page* Section::activatePage(Page::PageId pageId, Page::AnimationDirection direction) {
    Page* prevActivePage = activePages[activePageCount];
    Page* page = getPage(pageId);
    activePages[activePageCount++] = page;

    page->setAnimationDirection(direction);
    page->activate();
    return prevActivePage;
}

// Initialize custom pages
REPLACE void Section::init(Section::SectionId sectionId) {
    for (int i = 0; i < ARRAY_SIZE(extraPages); i++) {
        extraPages[i] = nullptr;
    }

    REPLACED(sectionId);
}

// Deinitialize and delete custom pages
REPLACE void Section::deinit() {

    // Ensure active pages are deactivated first
    popActivePages(0);

    // Remove regular pages
    for (int i = 0; i < ARRAY_SIZE(pages); i++) {
        Page* page = pages[i];
        if (page == nullptr)
            continue;

        page->onDeinit();
        delete page;
        pages[i] = nullptr;
    }

    // Remove custom pages
    for (int i = 0; i < ARRAY_SIZE(extraPages); i++) {
        Page* page = extraPages[i];
        if (page == nullptr)
            continue;

        page->onDeinit();
        delete page;
        extraPages[i] = nullptr;
    }

    // Original behaviour
    REPLACED();
}

// Section::calcActivation() patch
// Get page from the correct array
kmHookFn void CalcActivationFix(Section* self, Page::PageId pageId, Page::AnimationDirection animDirection) {
    Page* replacementPage = self->getPage(pageId);
    self->activePages[self->activePageCount++] = replacementPage;
    replacementPage->setAnimationDirection(animDirection);
    replacementPage->activate();
}

// Glue code
kmBranchDefAsm(0x80623118, 0x8062316C) {
    nofralloc

    // Call C++ code
    mr r3, r28
    mr r4, r29
    mr r5, r26
    bl CalcActivationFix
    blr
}
