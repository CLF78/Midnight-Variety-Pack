#include <kamek.h>
#include <game/ui/page/RaceCupSelectPage.h>
#include <game/ui/SectionManager.h>
#include <game/system/MultiDvdArchive.h>
#include <game/system/ResourceManager.h>
#include <stdlib/stdio.h>
#include "cupsystem/CupData.h"
#include "cupsystem/CupManager.h"
#if (CUSTOM_CUP_SYSTEM && CUSTOM_CUP_COURSE_SUPPORT)

// Externs for ASM code
extern "C" void SetupCups(RaceCupSelectPage* self);

// Cup screen initialization hook
kmBranchDefAsm(0x80841494, 0x80841498) {
    nofralloc

    // Call C++ function
    mr r3, r31
    bl SetupCups

    // Original instruction
    lwz r0, 0xE4(r1)
    blr
}

#if (CUP_COUNT < 9)

void SetupCups(RaceCupSelectPage* self) {

    // TODO select the correct cup
    // Disable the cup buttons if they exceed the cup count
    for (int i = CUP_COUNT; i < 8; i++) {
        u32 idx = CupManager::getCupButtonIdx(i);
        self->cupHolder.cupButtons[idx].hidden = true;
        self->cupHolder.cupButtons[idx].inputManager.unselectable = true;
    }
}
#else

void UpdateCupButton(PushButton* button, u32 cupIdx) {

    // Set the cup name id
    button->setMsgId(CupFile::cups[cupIdx].cupName, NULL);

    // Get the cup texture filename
    char buffer[64];
    snprintf(buffer, sizeof(buffer), "%s/%d.tpl", CUP_ICON_DIR_VS, cupIdx);

    // Get the cup texture itself
    size_t textureSize;
    void* cupTexture = ResourceManager::instance->getFile(MultiDvdArchive::MENU, buffer, &textureSize);

    // If the file does not exist, fall back to the "?" icon
    if (cupTexture == NULL) {
        button->setMatIcon("icon", "icon_08_hatena");
        button->setMatIcon("icon_light_01", "icon_08_hatena");
        button->setMatIcon("icon_light_02", "icon_08_hatena");
    } else {
        // TODO
    }
}

void UpdatePage(RaceCupSelectPage* self, u32 page) {

    // Set up loop
    u32 curColumn = page - 1;
    u32 totalPages = CupManager::getCupPageFromCup(CUP_COUNT);

    for (int i = 0; i < 8; i++) {

        // Wrap around the columns if we reach the rightmost page
        if (i % 2 == 0) {
            curColumn++;
            if (curColumn > totalPages)
                curColumn = 0;
        }

        // Get the cup's entry index
        u32 cupIdx = curColumn * 2 + i % 2;

        // If the cup index exceeds the cup count, hide the cup
        // Else show it
        u32 idx = CupManager::getCupButtonIdx(i);
        bool hide = cupIdx >= CUP_COUNT;
        self->cupHolder.cupButtons[idx].hidden = hide;
        self->cupHolder.cupButtons[idx].inputManager.unselectable = hide;

        // For all un-hidden cups, update their texture and text message
        if (!hide) {
            UpdateCupButton(&self->cupHolder.cupButtons[idx], cupIdx);
        }
    }
}

void SetupCups(RaceCupSelectPage* self) {

    // Get the previously selected track
    s32 curTrack = SectionManager::instance->globalContext->lastCourse;

    // If the screen is reached for the first time, default to the top left cup on page 0
    // Else do some math
    if (curTrack == -1) {
        self->selectedButton = 0;
        UpdatePage(self, 0);
        self->extension.curPage = 0;

    } else {
        // Get the cup's entry index in the cup list
        u32 cupIdx = CupManager::getCupFromTrack(curTrack);

        // Get the corresponding page
        u32 cupPage = CupManager::getCupPageFromCup(cupIdx);

        // Avoid wrapping the cups around in the initial display so it looks prettier
        u32 totalCupPages = CupManager::getCupPageFromCup(CUP_COUNT);
        if (totalCupPages - cupPage < 3)
            cupPage = totalCupPages - 3;

        // Update the page
        UpdatePage(self, cupPage);
        self->extension.curPage = cupPage;

        // Set the selected button accordingly
        self->selectedButton = CupManager::getCupButtonIdx(CupManager::getCupScreenPosition(cupIdx, cupPage));
    }
}

void RaceCupSelectArrow::onLeftArrowPress(SheetSelectControl* arrowPair, u32 localPlayerId) {
    RaceCupSelectPage* self = (RaceCupSelectPage*)SectionManager::instance->curSection->pages[Page::CUP_SELECT];
    self->extension.curPage--;
    if (self->extension.curPage < 0)
        self->extension.curPage = CupManager::getCupPageFromCup(CUP_COUNT);
    UpdatePage(self, self->extension.curPage);
}

void RaceCupSelectArrow::onRightArrowPress(SheetSelectControl* arrowPair, u32 localPlayerId) {
    RaceCupSelectPage* self = (RaceCupSelectPage*)SectionManager::instance->curSection->pages[Page::CUP_SELECT];
    self->extension.curPage++;
    if (self->extension.curPage > CupManager::getCupPageFromCup(CUP_COUNT))
        self->extension.curPage = 0;
    UpdatePage(self, self->extension.curPage);
}

#endif
#endif
