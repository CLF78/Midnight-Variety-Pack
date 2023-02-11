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
    snprintf(buffer, sizeof(buffer), CUP_ICON_DIR_VS "/%d.tpl", cupIdx);

    // Get the cup texture
    size_t textureSize;
    void* cupTexture = ResourceManager::instance->getFile(MultiDvdArchive::MENU, buffer, &textureSize);

    // If the file does not exist, fall back to the "?" icon
    if (cupTexture == NULL) {
        button->setMatIcon("icon", "icon_08_hatena");
        button->setMatIcon("icon_light_01", "icon_08_hatena");
        button->setMatIcon("icon_light_02", "icon_08_hatena");
    } else {
        // Get the icon pane again
        const char* iconPane = RaceCupSelectPage::getCupIconPane(buttonId);
        nw4r::lyt::Pane* pane = button->pictureSourceLayout->getPaneByName(iconPane);

        // Get its material
        nw4r::lyt::Material* mat = pane->GetMaterial();

        // Get the texmap entry
        nw4r::lyt::TexMap* texMap = mat->GetTexMapAry();

        // Replace the texture
        texMap->ReplaceImage(cupTexture, 0);

        // Set the icon pane
        button->setMatIcon("icon", iconPane);
        button->setMatIcon("icon_light_01", iconPane);
        button->setMatIcon("icon_light_02", iconPane);
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

extern "C" u32 GetTrackName(u32 buttonId, u32 track) {

    // Get the current page
    RaceCupSelectPage* page = (RaceCupSelectPage*)MenuPage::getMenuPage(Page::CUP_SELECT);
    u32 cupPos = CupManager::getCupPositionFromButton(buttonId);
    u32 cupIdx = CupManager::getCupIdxFromPosition(cupPos, page->extension.curPage);

    // Get the text message
    return CupManager::getTrackName(cupIdx, track);
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

// Preserve r4 in r28 and prevent updating it
kmWrite32(0x807E601C, 0x7C9C2378);
kmWrite32(0x807E6088, 0x60000000);
kmWrite32(0x807E60B4, 0x60000000);

// Update track names on selection change
kmCallDefAsm(0x807E608C) {
    mr r3, r28
    mr r4, r27
    b GetTrackName
}

// Preserve r3 in r27 and prevent updating it
kmWrite32(0x807E6114, 0x60000000);
kmWrite32(0x807E611C, 0x7C7B1B78);
kmWrite32(0x807E6184, 0x60000000);
kmWrite32(0x807E61B0, 0x60000000);

// Update track names on startup
kmCallDefAsm(0x807E6188) {
    mr r3, r27
    mr r4, r29
    b GetTrackName
}


#endif
#endif
