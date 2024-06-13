#include <common/Common.hpp>
#include <game/ui/Section.hpp>
#include <game/ui/page/BattleCupSelectPage.hpp>
#include <game/ui/page/BattleStageSelectPage.hpp>
#include <game/ui/page/WifiMenuPage.hpp>

///////////////////////////
// Page Expansion System //
///////////////////////////

// Apply page replacements
REPLACE_STATIC Page* Section::createPage(Page::PageID pageId) {
    switch (pageId) {

        // Expand battle cup select for custom cup system
        case Page::CUP_SELECT_BT:
            return new BattleCupSelectPageEx();

        case Page::COURSE_SELECT_BT:
            return new BattleStageSelectPageEx();

        // Expand main online menu for Wiimmfi console assignment message
        case Page::WIFI_MENU:
            return new WifiMenuPageEx();

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
