#include <kamek.h>
#include <game/ui/page/RaceCupSelectPage.h>
#include <game/ui/SectionManager.h>
#include <game/system/MultiDvdArchive.h>
#include <game/system/ResourceManager.h>
#include <stdlib/stdio.h>
#include "cupsystem/CupData.h"
#include "cupsystem/CupManager.h"
#if (CUSTOM_CUP_SYSTEM && CUSTOM_CUP_COURSE_SUPPORT)
#if (CUP_COUNT < 9)

// TODO rewrite
void SetupCups(RaceCupSelectPage* self) {

    // TODO select the correct cup
    // Disable the cup buttons if they exceed the cup count
    for (int i = CUP_COUNT; i < 8; i++) {
        u32 idx = CupManager::getCupButtonIdx(i);
        self->cupHolder.cupButtons[idx].hidden = true;
        self->cupHolder.cupButtons[idx].inputManager.unselectable = true;
    }
}

// Cup screen initialization hook
// TODO rewrite
kmBranchDefAsm(0x80841494, 0x80841498) {
    nofralloc

    // Call C++ function
    mr r3, r31
    bl SetupCups

    // Original instruction
    lwz r0, 0xE4(r1)
    blr
}

#else

extern "C" void ReplaceCupIcon(int buttonId, PushButton* button) {

    // Get the cup index
    RaceCupSelectPage* page = (RaceCupSelectPage*)MenuPage::getMenuPage(Page::CUP_SELECT);
    u32 cupPos = CupManager::getCupPositionFromButton(buttonId);
    u32 cupIdx = CupManager::getCupIdxFromPosition(cupPos, page->extension.curPage);

    // Insted of replacing the texture, hide the cup entirely if it exceeds the maximum index
    bool hide = (cupIdx >= CUP_COUNT);
    button->hidden = hide;
    button->inputManager.unselectable = hide;

    // Return early since this cup won't exist anyway
    if (hide)
        return;

    // Get the cup texture name
    char buffer[64];
    snprintf(buffer, sizeof(buffer), "%s/%d.tpl", CUP_ICON_DIR_VS, cupIdx);

    // Get the cup texture
    size_t textureSize;
    void* cupTexture = ResourceManager::instance->getFile(MultiDvdArchive::MENU, buffer, &textureSize);

    // If the file does not exist, fall back to the "?" icon
    if (cupTexture == NULL) {
        button->setMatIcon("icon", "icon_08_hatena");
        button->setMatIcon("icon_light_01", "icon_08_hatena");
        button->setMatIcon("icon_light_02", "icon_08_hatena");
    } else {
        // Set the icon pane
        const char* iconPane = RaceCupSelectPage::getCupIconPane(buttonId);
        button->setMatIcon("icon", iconPane);
        button->setMatIcon("icon_light_01", iconPane);
        button->setMatIcon("icon_light_02", iconPane);

        // Get the icon pane again
        nw4r::lyt::Pane* pane = button->pictureSourceLayout->getPaneByName(iconPane);

        // Get its material
        nw4r::lyt::Material* mat = pane->GetMaterial();

        // Get the material's texture count
        u8 texMapIdx = mat->flags >> 28;

        // Get the texmap entry
        nw4r::lyt::TexMap* texMap = &mat->GetTexMapAry()[texMapIdx-1];

        // Replace the texture
        texMap->ReplaceImage(cupTexture, 0);
    }
}

extern "C" u16 GetCupName(u32 cupButtonId) {
    RaceCupSelectPage* page = (RaceCupSelectPage*)MenuPage::getMenuPage(Page::CUP_SELECT);
    u32 cupPos = CupManager::getCupPositionFromButton(cupButtonId);
    u32 cupIdx = CupManager::getCupIdxFromPosition(cupPos, page->extension.curPage);

    // Failsafe
    if (cupIdx >= CUP_COUNT)
        return 0;

    return CupFile::cups[cupIdx].cupName;
}

extern "C" u32 GetDefaultButton(s32 track, RaceCupSelectPage* self) {
    u32 cupIdx = CupManager::getCupIdxFromTrack(track);
    u32 pos = CupManager::getCupPositionFromIdx(cupIdx, self->extension.curPage);
    return CupManager::getCupButtonFromPosition(pos);
}

// Bypass cup unlock check on startup
kmWrite32(0x807E58F8, 0x480000C4);

// Replace cup names on startup
kmCall(0x807E59E8, GetCupName);

// Replace cup icons on startup
kmBranchDefAsm(0x807E5ABC, 0x807E5B00) {
    nofralloc

    // Call C++ function
    mr r4, r31
    bl ReplaceCupIcon

    blr
}

// Set default button
kmCallDefAsm(0x80841170) {
    nofralloc

    mr r4, r31
    b GetDefaultButton
}

void RaceCupSelectArrow::onLeftArrowPress(SheetSelectControl* arrowPair, u32 localPlayerId) {

    // Get the parent screen
    RaceCupSelectPage* self = (RaceCupSelectPage*)MenuPage::getMenuPage(Page::CUP_SELECT);

    // Update the page number
    self->extension.curPage--;
    if (self->extension.curPage < 0)
        self->extension.curPage = CupManager::getMaxCupPage();

    // Update each button
    for (int i = 0; i < 8; i++) {
        PushButton* cupButton = &self->cupHolder.cupButtons[i];
        u32 cupName = GetCupName(i);
        cupButton->setMsgId(cupName, NULL);
        ReplaceCupIcon(i, cupButton);
    }

    // Remove the text in the course selection
    for (int i = 0; i < 4; i++) {
        self->cupHolder.courseHolder.courseNames[i].resetText();
    }
}

void RaceCupSelectArrow::onRightArrowPress(SheetSelectControl* arrowPair, u32 localPlayerId) {

    // Get the parent screen
    RaceCupSelectPage* self = (RaceCupSelectPage*)MenuPage::getMenuPage(Page::CUP_SELECT);

    self->extension.curPage++;
    if (self->extension.curPage > CupManager::getMaxCupPage())
        self->extension.curPage = 0;

    // Update each button
    for (int i = 0; i < 8; i++) {
        PushButton* cupButton = &self->cupHolder.cupButtons[i];
        u32 cupName = GetCupName(i);
        cupButton->setMsgId(cupName, NULL);
        ReplaceCupIcon(i, cupButton);
    }

    // Remove the text in the course selection
    for (int i = 0; i < 4; i++) {
        self->cupHolder.courseHolder.courseNames[i].resetText();
    }
}

#endif
#endif
