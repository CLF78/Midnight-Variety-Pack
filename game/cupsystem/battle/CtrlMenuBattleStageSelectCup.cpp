#include <kamek.h>
#include <game/ui/page/BattleCupSelectPage.h>
#include <game/ui/page/BattleStageSelectPage.h>
#include <game/ui/ctrl/CtrlMenuBattleStageSelectCup.h>
#include <game/ui/ControlLoader.h>
#include "cupsystem/CupManager.h"

// Update various counters
kmWrite8(0x807E131B, 8); // Layout children count
kmWrite8(0x807E141F, 8); // Loop counter
kmWrite8(0x807E1527, 8); // Another loop counter

// Replace cup button BRCTR with the VS one
kmCallDefCpp(0x807E13A4, void, ControlLoader* self, const char* dirname,
             const char* filename, const char* variant, const char** anims) {

    self->load(dirname, "CourseSelectCup", variant, anims);
}

// Replace the initialization loop variable to get the six additional buttons
kmHookFn CtrlMenuBattleStageSelectCupSub* GetCupButton(u32 idx) {
    return BattleStageSelectPage::getPage(Page::COURSE_SELECT_BT)->getCupButton(idx);
}

// Glue code
kmBranchDefAsm(0x807E1334, 0x807E1338) {
    nofralloc

    // Call C++ code
    mr r3, r27
    bl GetCupButton

    // Replace r28 and execute original instruction
    mr r28, r3
    mr r6, r27
    blr
}

// Replace isSelected store to account for the new buttons
kmWrite32(0x807E1520, 0x981DFFFC);

// Replace cup name
kmHookFn u16 GetCupName(int cupButtonId) {
    BattleCupSelectPage* page = BattleCupSelectPage::getPage(Page::CUP_SELECT_BT);
    u32 cupIdx = CupManager::getCupIdxFromButton(cupButtonId, page->extension.curPage, true);
    return CupManager::GetCupArray(true)[cupIdx].cupName;
};

// Glue code
kmCall(0x807E13AC, GetCupName);

// Replace cup icon
kmHookFn void ReplaceCupIcon(int buttonId, LayoutUIControl* button) {
    BattleCupSelectPage* page = BattleCupSelectPage::getPage(Page::CUP_SELECT_BT);
    CupManager::updateCupButton(buttonId, button, page->extension.curPage, true);
}

// Glue code
kmBranchDefAsm(0x807E13C4, 0x807E1408) {
    nofralloc

    // Call C++ function
    mr r4, r28
    bl ReplaceCupIcon
    blr
}

// Update cup name and icon when initSelf is called
kmHookFn CtrlMenuBattleStageSelectCupSub* UpdateCupButton(u32 idx) {
    CtrlMenuBattleStageSelectCupSub* btn = GetCupButton(idx);
    btn->setMsgId(GetCupName(idx), nullptr);
    ReplaceCupIcon(idx, btn);
    return btn;
}

// Glue code
kmBranchDefAsm(0x807E1478, 0x807E147C) {
    nofralloc

    // Call C++ code
    mr r3, r26
    bl UpdateCupButton

    // Replace r29 and execute original instruction
    mr r29, r3
    subf r0, r26, r27
    blr
}
