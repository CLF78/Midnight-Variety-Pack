#include <kamek.h>
#include <game/ui/page/BattleCupSelectPage.h>
#include <game/ui/ControlLoader.h>
#include "cupsystem/CupManager.h"

// Skip boundary check
kmWrite32(0x807E0B8C, 0x48000014);

// Update counts and indexes
kmWrite8(0x807E09CB, 9); // Child count
kmWrite8(0x807E0AAF, 8); // Loop counter
kmWrite8(0x807E0C5F, 8); // Another loop counter
kmWrite8(0x807E0AC3, 8); // MovieLayout child index

// Replace base BRCTR with the VS one
kmCallDefCpp(0x807E09C0, void, ControlLoader* self, const char* dirname,
             const char* filename, const char* variant, const char** anims) {

    self->load(dirname, "CupSelectNULL", variant, anims);
}

// Replace cup button BRCTR with the VS one
kmCallDefCpp(0x807E0A20, void, PushButton* self, const char* dirname, const char* filename,
             const char* variant, u32 playerFlags, bool param_6, bool inaccessible) {

    self->initLayout(dirname, "CupSelectCup", variant, playerFlags, param_6, inaccessible);
}

// Replace the initialization loop variable to get the six additional buttons
kmHookFn PushButton* GetCupButton(u32 idx) {
    return BattleCupSelectPage::getPage(Page::CUP_SELECT_BT)->getCupButton(idx);
}

// Glue code for initLayout
kmBranchDefAsm(0x807E09F4, 0x807E09F8) {
    nofralloc

    // Call C++ code
    mr r3, r28
    bl GetCupButton

    // Replace r30 and execute original instruction
    mr r30, r3
    mr r3, r25
    blr
}

// Glue code for init
kmBranchDefAsm(0x807E0BC0, 0x807E0BC4) {
    nofralloc

    // Call C++ code
    mr r3, r25
    bl GetCupButton

    // Replace r27 and execute modified original instruction
    mr r27, r3
    lwz r0, 0x240(r27)
    blr
}

// Store/load the button id using the returned button instead of the loop variable
kmWrite32(0x807E0A24, 0x939E0240);
kmWrite32(0x807E0BE8, 0x801B0240);

// Replace cup name
kmCallDefCpp(0x807E0A2C, u16, int cupButtonId) {
    BattleCupSelectPage* page = BattleCupSelectPage::getPage(Page::CUP_SELECT_BT);
    u32 cupIdx = CupManager::getCupIdxFromButton(cupButtonId, page->extension.curPage);
    return CupManager::GetCupArray()[cupIdx].cupName;
};

// Replace cup icon
kmHookFn void ReplaceCupIcon(int buttonId, PushButton* button) {
    BattleCupSelectPage* page = BattleCupSelectPage::getPage(Page::CUP_SELECT_BT);
    CupManager::updateCupButton(buttonId, button, page->extension.curPage);
}

// Glue code
kmBranchDefAsm(0x807E0A44, 0x807E0A88) {
    nofralloc

    // Call C++ function
    mr r4, r30
    bl ReplaceCupIcon
    blr
}
