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
        u32 localPlayerCount = RaceConfig::instance->raceScenario.localPlayerCount;
        count += MessageQueue::instance.GetMessageCount(localPlayerCount) * localPlayerCount;
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
    MessageQueue::instance.Clear();
    if (controls & MESSAGE_QUEUE) {

        // Set the local player count and enable the queue
        u32 localPlayerCount = RaceConfig::instance->raceScenario.localPlayerCount;
        MessageQueue::instance.localPlayerCount = localPlayerCount;
        MessageQueue::instance.queueEnabled = true;

        // Initialize the messages
        for (int i = 0; i < localPlayerCount; i++) {
            for (int j = 0; j < MessageQueue::instance.GetMessageCount(localPlayerCount); j++) {

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
