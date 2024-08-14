#include "eggException.hpp"
#include <egg/core/eggThread.hpp>
#include <game/system/SystemManager.hpp>
#include <revolution/ax.h>
#include <revolution/kpad.h>
#include <revolution/os/OS.h>
#include <revolution/os/OSLog.h>
#include <revolution/os/OSThread.h>
#include <revolution/pad.h>

///////////////////////
// Exception Handler //
///////////////////////

// Update exception console settings
kmListHookDefCpp(BootHook) {

    // Enable automatic text wrapping
    EGG::Exception::sConsoleHandle->attributes &= ~nw4r::db::ConsoleHead::CONSOLE_ATTR_NO_TURNOVER_LINE;

    // Do not require scrolling horizontally to read the contents
    EGG::Exception::sConsoleHandle->width = 50;
}

// Threshold functions for circle pads
// Credits: Vabold
bool CheckGCStickThreshold(s8 stick) {
    return (stick >= 27 || stick <= -26);
}

bool CheckCLStickThreshold(s16 stick) {
    return (stick >= 161 || stick <= -160);
}

// Patch the exception callback to add some new features (code lifted from mkw-sp):
// - Mute game audio to avoid spooking players (and devs)
// - Remove button combo requirement to display the exception handler
// - Disable initial automatic scrolling
// - Disable horizontal scrolling
// - Limit vertical scrolling to filled lines
// - Support controllers other than Wiimote
// - Allow returning to the Wii Menu on HOME/START button press
// Credits: Star, Vabold
REPLACE bool EGG::ExceptionCallback(nw4r::db::ConsoleHead* console, void* arg) {

    // Mute game audio
    AXSetMasterVolume(0);

    // Stripped original code
    if (!console) {
        OSReport("No console.\n");
        return false;
    }

    OSReport("Canceling all threads...\n");
    EGG::Thread::kandoTestCancelAllThread();
    OSReport("Done.\n");

    OSDisableInterrupts();
    OSDisableScheduler();
    OSEnableInterrupts();

    // Subtract displayed line count to avoid scrolling into nothingness
    const s32 lineCount = console->ringTopLineCnt;
    const s32 totalLines = nw4r::db::Console_GetTotalLines(console) - console->displayLineCount;

    // Initial console draw
    console->isVisible = true;
    console->topLineNumber = lineCount;
    nw4r::db::Console_DrawDirect(console);

    // Get controller type
    u32 controllerType;
    const s32 err = WPADProbe(0, &controllerType);
    const bool isClassic = (err == WPAD_ERR_NONE && controllerType == WPAD_TYPE_CLASSIC);

    while (true) {
        KPADStatus wiimoteStatus;
        PADStatus gamecubeStatus[PAD_CONTROLLER_COUNT];
        KPADUnifiedWpadStatus unifiedStatus;

        // Get inputs
        KPADRead(WPAD_CONTROLLER_1, &wiimoteStatus, 1);
        PADRead(gamecubeStatus);
        PADClampCircle(gamecubeStatus);
        if (isClassic)
            KPADGetUnifiedWpadStatus(WPAD_CONTROLLER_1, &unifiedStatus, 1);

        // Return to the menu on HOME/START press
        if (OSGetCurrentThread()) {
            if (wiimoteStatus.buttonsHeld & WPAD_BUTTON_HOME ||
                gamecubeStatus[PAD_CONTROLLER_1].buttons & PAD_BUTTON_START ||
                (isClassic && unifiedStatus.cl.buttons & WPAD_CL_BUTTON_HOME)) {
                    SystemManager::ReturnToMenu();
            }
        }

        // Check for UP/DOWN inputs
        bool down = (!!(wiimoteStatus.buttonsHeld & WPAD_BUTTON_DOWN) ||
                     !!(gamecubeStatus[PAD_CONTROLLER_1].buttons & PAD_BUTTON_DOWN));
        bool up = (!!(wiimoteStatus.buttonsHeld & WPAD_BUTTON_UP) ||
                   !!(gamecubeStatus[PAD_CONTROLLER_1].buttons & PAD_BUTTON_UP));

        if (isClassic) {
            up |= !!(unifiedStatus.cl.buttons & WPAD_CL_BUTTON_UP);
            down |= !!(unifiedStatus.cl.buttons & WPAD_CL_BUTTON_DOWN);
        }

        // Check Gamecube Controller stick
        if (CheckGCStickThreshold(gamecubeStatus[PAD_CONTROLLER_1].stickY)) {
            if (gamecubeStatus[PAD_CONTROLLER_1].stickY < 0)
                down = true;
            else
                up = true;
        }

        // Check Classic Controller left stick
        if (isClassic && CheckCLStickThreshold(unifiedStatus.cl.leftStickY)) {
            if (unifiedStatus.cl.leftStickY < 0)
                down = true;
            else
                up = true;
        }

        // Wait 100 milliseconds between inputs
        OSWait(OSMillisecondsToTicks(100));

        // Scroll in the desired direction (but not if we are at the top/bottom)
        s32 currentTopLine = console->topLineNumber;
        const s32 prevTopLine = currentTopLine;

        if (down)
            currentTopLine = MIN(currentTopLine + 1, totalLines);
        else if (up)
            currentTopLine = MAX(currentTopLine - 1, lineCount);

        // Redraw the screen (if the contents have changed)
        if (currentTopLine != prevTopLine) {
            console->topLineNumber = currentTopLine;
            nw4r::db::Console_DrawDirect(console);
        }
    }
}
