#include "MessageQueue.hpp"
#include <platform/string.h>

MessageQueue MessageQueue::instance;

kmListHookDefCpp(RaceStartHook) {
    MessageQueue::instance.ClearText();
}

void MessageQueue::Clear() {
    for (u32 i = 0; i < ARRAY_SIZE(entries); i++) {
        entries[i].Clear();
    }
}

void MessageQueue::ClearText() {
    for (u32 i = 0; i < ARRAY_SIZE(entries); i++) {
        entries[i].ClearText();
    }
}

u32 MessageQueue::GetMessageCount(u32 localPlayerCount) const {
    switch (localPlayerCount) {
        case 1:
            return ARRAY_SIZE(entries);
        default:
            return ARRAY_SIZE(entries) / 3;
    }
}

void MessageQueue::Push(u32 msgId, MessageInfo* msgInfo) {

    // If not enabled, bail
    if (!queueEnabled) {
        return;
    }

    // Push existing entries forwards
    LOG_DEBUG("Pushing message %d to the queue...", msgId);
    for (u32 i = GetMessageCount(localPlayerCount) - 1; i > 0; i--) {
        Entry* src = &entries[i - 1];
        Entry* dst = &entries[i];

        if (src->msgId == 0) {
            continue;
        }

        dst->msgId = src->msgId;
        dst->msgInfo = src->msgInfo;
        dst->UpdateText();

        dst->display->timer = src->display->timer;
        dst->display->state = src->display->state;
        dst->display->hidden = src->display->hidden;
        dst->display->textBox->alpha = src->display->textBox->alpha;

        if (dst->display->screenState == CtrlRaceBase::DISPLAYED) {
            dst->display->screenState = CtrlRaceBase::ENTERING_SCREEN;
            dst->display->delta = 0.5f;
        }
    }

    // Add the new message
    Entry* newEntry = &entries[0];
    newEntry->msgId = msgId;

    if (msgInfo) {
        newEntry->msgInfo = *msgInfo;
    }
    else {
        newEntry->msgInfo.reset();
    }

    newEntry->UpdateText();
    newEntry->display->showMessage();
}
