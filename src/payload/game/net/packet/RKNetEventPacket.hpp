#pragma once
#pragma pack(push, 1)

struct RKNetEVENTPacket {
    struct EventInfo {
        enum EventType {
            EVENT_TYPE_NONE,
            EVENT_TYPE_ITEM_USED,
            EVENT_TYPE_ITEM_THROWN,
            EVENT_TYPE_ITEM_TRAIL_HIT,
            EVENT_TYPE_ITEM_HIT,
            EVENT_TYPE_ITEM_LOCKED_ON,
            EVENT_TYPE_UNK_6,
            EVENT_TYPE_ITEM_DROPPED,
        };

        u8 eventType : 3;
        u8 itemObject : 5;
    };

    EventInfo eventInfo[0x18];
    u8 _18[0xF8 - 0x18];
};
size_assert(RKNetEVENTPacket, 0xF8);
#pragma pack(pop)
