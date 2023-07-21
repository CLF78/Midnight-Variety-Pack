#include <kamek.h>
#include <game/ui/page/ClassSelectPage.h>
#include <game/ui/SectionManager.h>
#include <game/system/RaceConfig.h>

// Replace CCs used for each button
kmWrite32(0x808AD168, RaceConfig::Settings::CC_150);
kmWrite32(0x808AD16C, RaceConfig::Settings::CC_200);
kmWrite32(0x808AD170, RaceConfig::Settings::CC_500);

// Skip creating the mirror mode button
kmWrite32(0x8083F4C4, 0x48000054);
kmWrite32(0x8083F65C, 0x4800007C);

// Hide the "change vehicles" option
kmWrite32(0x8083F88C, 0x48000064);

// Make the "change vehicles" option non-functional
kmWrite32(0x8083FBC8, 0x4E800020);

// Force kart+bike movies
kmCallDefAsm(0x8083F70C) {
    nofralloc

    // Only fix the 50cc and 100cc buttons
    cmpwi r29, 1
    bgt noChange

    // Add 4 to the child index to use the alternate movies/text
    addi r29, r29, 4

    // Original instruction
    noChange:
    xoris r3, r29, 0x8000
    blr
}

// Replace CC text with custom messages
kmCallDefCpp(0x8083F784, void, LayoutUIControl* button, u32 buttonId, MessageInfo* msgInfo) {
    button->setMsgId(20000 + (buttonId & 3), msgInfo);
}

// Glue code
kmWrite32(0x8083F77C, 0x7FA4EB78);

// Fix the back button when exiting TT mode after changing course/character
kmCallDefAsm(0x80626A10) {
    nofralloc

    // Use r11 to avoid clobbering r12
    li r11, 0x69
    stw r11, 0x3EC(r28)
    blr
}

// Select the correct button when exiting TT mode
kmCallDefCpp(0x8083F52C, void, ClassSelectPage* page) {

    // Original call
    page->callOnInit();

    // If we are not returning from TT mode, bail
    u32 section = SectionManager::instance->curSection->sectionID;
    if (section != Section::MENUSINGLE_FROM_TT_CHANGE_CHAR && section != Section::MENUSINGLE_FROM_TT_CHANGE_COURSE)
        return;

    // Get each button id
    for (int i = 0; i < page->buttonCount; i++) {
        PushButton* btn = page->buttons[i];
        int buttonId = btn->buttonId;

        // Skip all irrelevant buttons
        if (buttonId < 0 || buttonId > 2)
            continue;

        // If the engine class matches the current button's, set the button and return
        u32 engineClass = ClassSelectPage::engineClasses[buttonId];
        if (RaceConfig::instance->menuScenario.settings.engineClass == engineClass) {
            btn->selectDefault(0);
            return;
        }
    }
}
