#include <common/Common.hpp>
#include <game/system/RaceConfig.hpp>
#include <game/ui/page/RacePage.hpp>
#include <midnight/race/CtrlRaceMessageDisplay.hpp>
#include <midnight/race/MessageQueue.hpp>

///////////////////////////////
// Race UI Element Injection //
///////////////////////////////

REPLACE int GpRacePage::getControls() const {
    return REPLACED() | RacePage::MESSAGE_QUEUE;
}

REPLACE int TtRacePage::getControls() const {
    return REPLACED() | RacePage::MESSAGE_QUEUE;
}

REPLACE int SingleVsRacePage::getControls() const {
    return REPLACED() | RacePage::MESSAGE_QUEUE;
}

REPLACE int WifiSingleVsRacePage::getControls() const {
    return REPLACED() | RacePage::MESSAGE_QUEUE;
}

REPLACE int WifiFriendVsRacePage::getControls() const {
    return REPLACED() | RacePage::MESSAGE_QUEUE;
}

REPLACE int WifiFriendTeamVsRacePage::getControls() const {
    return REPLACED() | RacePage::MESSAGE_QUEUE;
}

REPLACE int MultiVsRacePage::getControls() const {
    return REPLACED() | RacePage::MESSAGE_QUEUE;
}

REPLACE int WifiMultiVsRacePage::getControls() const {
    return REPLACED() | RacePage::MESSAGE_QUEUE;
}

REPLACE int SingleBtRacePage::getControls() const {
    return REPLACED() | RacePage::MESSAGE_QUEUE;
}

REPLACE int MultiBtRacePage::getControls() const {
    return REPLACED() | RacePage::MESSAGE_QUEUE;
}

REPLACE int ToRacePage::getControls() const {
    return REPLACED() | RacePage::MESSAGE_QUEUE;
}

REPLACE u8 RacePage::getControlCount(Controls controls) {

    // Get original count
    u8 count = REPLACED(controls);

    // Add the message queue if enabled
    if (controls & MESSAGE_QUEUE) {
        count += ARRAY_SIZE(MessageQueue::instance.entries[0]) * RaceConfig::instance->raceScenario.localPlayerCount;
    }

    // Return modified count
    return count;
}

REPLACE void RacePage::initControls(Controls controls) {

    // Call original function
    REPLACED(controls);

    // Find the first empty child index in the control group
    u8 currChildIdx = 0;
    for (int i = 0; i < controlGroup.dataSize; i++) {
        if (controlGroup.data[i] == nullptr) {
            currChildIdx = i;
            break;
        }
    }

    // If no child is found, bail
    if (currChildIdx == 0)
        return;

    // Initialize the message queue
    if (controls & MESSAGE_QUEUE) {

        // Reset the queue and set the local player count
        u32 localPlayerCount = RaceConfig::instance->raceScenario.localPlayerCount;
        MessageQueue::instance.Clear();
        MessageQueue::instance.localPlayerCount = localPlayerCount;

        for (int i = 0; i < localPlayerCount; i++) {
            for (int j = 0; j < ARRAY_SIZE(MessageQueue::instance.entries[0]); j++) {

                // Insert the display
                CtrlRaceMessageDisplay* display = new CtrlRaceMessageDisplay();
                insertChild(currChildIdx++, display, 0);

                // Load it and set it in the message queue
                display->load(localPlayerCount, i, j);
                MessageQueue::instance.entries[i][j].display = display;
            }
        }
    }
}
