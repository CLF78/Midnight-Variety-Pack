#include <game/ui/SectionManager.hpp>
#include "MenuPage.hpp"

///////////////////////
// Custom Cup System //
///////////////////////

// MenuPage::onRefocus() patch
// Prevent the game from rewriting the last selected course/stage
kmBranch(0x80837DB0, 0x80837E2C);

///////////////////////////
// Page Expansion System //
///////////////////////////

// Allow transitioning to custom menu pages
REPLACE void MenuPage::loadPrevPageWithDelayById(PageId pageId, float delay) {
    nextPageId = pageId;
    MenuPage* prevPage = (MenuPage*)SectionManager::instance->curSection->getPage(pageId);
    if (prevPage) {
        prevPage->nextPageId = this->pageId;
    }

    if (!locked && pageState == STATE_ACTIVE) {
        replace(ANIM_PREV, delay);
        locked = true;
    }
}

// Allow transitioning to custom menu pages
REPLACE void MenuPage::loadPrevPageWithDelay(float delay) {
    loadPrevPageWithDelayById(prevPageId, delay);
}

// Allow transitioning to custom menu pages
REPLACE void MenuPage::loadPrevPageById(PageId pageId, PushButton* button) {
    loadPrevPageWithDelayById(pageId, button->getDelay());
}

// Allow transitioning to custom menu pages
REPLACE void MenuPage::loadPrevPage(PushButton* button) {
    loadPrevPageWithDelayById(prevPageId, button->getDelay());
}

// Allow transitioning to custom menu pages
REPLACE void MenuPage::loadNextPageWithDelayById(PageId pageId, float delay) {
    nextPageId = pageId;
    MenuPage* nextPage = (MenuPage*)SectionManager::instance->curSection->getPage(pageId);
    if (nextPage) {
        nextPage->prevPageId = this->pageId;
    }

    if (!locked && pageState == STATE_ACTIVE) {
        replace(ANIM_NEXT, delay);
        locked = true;
    }
}

// Allow transitioning to custom menu pages
REPLACE void MenuPage::loadNextPageById(PageId pageId, PushButton* button) {
    loadNextPageWithDelayById(pageId, button->getDelay());
}
