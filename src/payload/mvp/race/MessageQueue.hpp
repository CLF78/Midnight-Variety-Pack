#include <common/Common.hpp>
#include <game/ui/MessageInfo.hpp>
#include <mvp/race/CtrlRaceMessageDisplay.hpp>

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
    int GetMessageCount(u32 localPlayerCount);

    Entry entries[4][6]; // 6 per player at most
    u32 localPlayerCount;
    bool queueEnabled;

    static MessageQueue instance;
};
