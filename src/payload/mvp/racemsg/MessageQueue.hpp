#pragma once
#include "CtrlRaceMessageDisplay.hpp"
#include <game/ui/MessageInfo.hpp>

class MessageQueue {
public:
    struct Entry {

        void UpdateText() {
            if (display) {
                display->setText(msgId, &msgInfo);
            }
        }

        void Clear() {
            msgId = 0;
            msgInfo.reset();
            display = nullptr;
        }

        u32 msgId;
        MessageInfo msgInfo;
        CtrlRaceMessageDisplay* display;
    };

    MessageQueue() { Clear(); }
    void Clear();
    void Push(u32 msgId, MessageInfo* msgInfo, u32 playerFlags = 0xFFFFFFFF);
    u32 GetMessageCount(u32 localPlayerCount) const;

    Entry entries[2][6]; // 6 per player at most
    u32 localPlayerCount;
    bool queueEnabled;

    static MessageQueue instance;
};
