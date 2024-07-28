#pragma once
#include <common/Common.hpp>
#pragma pack(push, 1)

struct RKNetROOMPacket {

    // TODO expand this so we can send setting updates live
    enum MessageType {
        MSG_NONE,
        MSG_ROOM_START,        // param1: event type, param2: none
        MSG_REGISTER_FRIEND,   // param1: none, param2: none
        MSG_JOIN,              // param1: none, param2: none
        MSG_CHAT,              // param1: message id, param2: queue position

        // Custom message types onwards
        // MSG_UPDATE_SETTING, // param1: setting value, param2: setting type
    };

    // TODO we can consider adding new event types in the future
    enum EventType {
        EVENT_GP,
        EVENT_TEAM_GP,
        EVENT_BALLOON_BT,
        EVENT_COIN_BT,
    };

    u8 msgType;
    u16 param1;
    u8 param2;
};
size_assert(RKNetROOMPacket, 0x4);

// TODO remove this if we expand the packet
union RKNetROOMPacketRaw {
    RKNetROOMPacketRaw(u32 data) : raw(data) {}

    RKNetROOMPacket packet;
    u32 raw;
};

#pragma pack(pop)
