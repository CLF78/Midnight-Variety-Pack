#include <kamek.h>
#include <game/system/RaceConfig.h>
#include <game/ui/Page.h>

// Change GP mode to skip the CC selector screen and go to character select
kmWrite8(0x808AE1FB, Page::CHARACTER_SELECT);

// Change TT mode to go to the CC selector screen
kmWrite8(0x808AE1FF, Page::GP_CLASS_SELECT);

// Write some required settings that the CC selector screen used to do for us
kmHookFn void ApplyGPSettings() {
    RaceConfig::instance->menuScenario.settings.engineClass = RaceConfig::Settings::CC_150;
    RaceConfig::instance->menuScenario.settings.cpuMode = RaceConfig::Settings::CPU_HARD;
}

// Glue code
kmBranchDefAsm(0x8084F4BC, 0x8084F510) {
    nofralloc

    // Check if the clicked button is the GP mode one
    cmpwi r4, 0
    bne+ end

    // If so, apply a couple settings
    bl ApplyGPSettings

    // Restore r4 and return
    end:
    lwz r4, 0x240(r31)
    blr
}
