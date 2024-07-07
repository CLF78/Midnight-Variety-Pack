#include <common/Common.hpp>
#include <midnight/race/MessageQueue.hpp>
#include <platform/string.h>

MessageQueue MessageQueue::instance;

void MessageQueue::Clear() {

    for (int i = 0; i < ARRAY_SIZE(entries); i++) {
        for (int j = 0; j < ARRAY_SIZE(entries[0]); j++) {
            entries[i][j].Clear();
        }
    }

    localPlayerCount = 0;
}

void MessageQueue::Push(u32 msgId, MessageInfo* msgInfo, u32 playerFlags) {

    // Parse each player
    LOG_DEBUG("Pushing message %d to the queue...", msgId);
    for (int i = 0; i < localPlayerCount; i++) {

        // Skip players who are not affected
        if (playerFlags & (1 << i)) {

            // Push existing entries forwards
            for (int j = ARRAY_SIZE(entries[0]) - 1; j > 0; j--) {
                Entry* src = &entries[i][j-1];
                Entry* dst = &entries[i][j];

                if (src->msgId == 0)
                    continue;

                dst->msgId = src->msgId;
                dst->msgInfo = src->msgInfo;
                dst->UpdateText();

                dst->display->timer = src->display->timer;
                dst->display->state = src->display->state;
                dst->display->hidden = src->display->hidden;
                dst->display->textBox->alpha = src->display->textBox->alpha;
            }

            // Add the new message
            Entry* newEntry = &entries[i][0];
            newEntry->msgId = msgId;

            if (msgInfo) {
                newEntry->msgInfo = *msgInfo;
            } else {
                newEntry->msgInfo.reset();
            }

            newEntry->UpdateText();
            newEntry->display->showMessage();
        }
    }
}
