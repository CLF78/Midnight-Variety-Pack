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

        void ClearText() {
            msgId = 0;
            msgInfo.reset();
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
    void ClearText();
    void Push(u32 msgId, MessageInfo* msgInfo);
    u32 GetMessageCount(u32 localPlayerCount) const;

    Entry entries[6]; // reduced to 3 in multiplayer
    u32 localPlayerCount;
    bool queueEnabled;

    static MessageQueue instance;
};
