#include <common/Common.hpp>
#include <game/system/RaceManager.hpp>
#include <game/ui/ControlLoader.hpp>
#include <midnight/race/CtrlRaceMessageDisplay.hpp>
#include <platform/stdio.h>

RUNTIME_TYPE_INFO_DEFINE(CtrlRaceMessageDisplay, LayoutUIControl);

void CtrlRaceMessageDisplay::load(u32 localPlayerCount, u32 localPlayerIdx, u32 queuePos) {

    // Load the correct BRCTR variant
    char buffer[32];
    snprintf(buffer, sizeof(buffer), "Message_%d_%d_%d", localPlayerCount, localPlayerIdx, queuePos);

    ControlLoader loader(this);
    loader.load("game_image", "MessageQueueMessage", buffer, nullptr);

    // Get the text box and reset the text
    textBox = mainLayout.getPaneByName("TextBox_00");
    resetText();
}

void CtrlRaceMessageDisplay::calcSelf() {

    // Constants for frame durations
    const int stayFrames = 60 * 4;
    const int fadeOutFrames = 60 * 1;

    switch (state) {

        // Display the text in the VISIBLE state
        case VISIBLE:
            if (++timer >= stayFrames) {
                state = FADE_OUT;
                timer = 0;
            }
            break;

        // Update the alpha while in the FADE_OUT state
        case FADE_OUT:
            textBox->alpha = (u8)(255.0f * (1.0f - ((float)timer / (float)fadeOutFrames)));
            if (++timer >= fadeOutFrames) {
                hidden = true;
                state = INVISIBLE;
            }
            break;

        // Do not do anything when in the INVISIBLE state
        case INVISIBLE:
            break;
    }
}

void CtrlRaceMessageDisplay::showMessage() {
    timer = 0;
    state = CtrlRaceMessageDisplay::VISIBLE;
    hidden = false;
}
