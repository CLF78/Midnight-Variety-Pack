#pragma once

struct RKNetITEMPacket {

    enum State {
        STATE_NOITEM,
        STATE_DECIDED,
        STATE_FINALISED,
        STATE_RESET,
        STATE_3_LEFT,
        STATE_2_LEFT,
        STATE_1_LEFT,
        STATE_REJECTED,
    };

    enum TrailState {
        TRAIL_STATE_NOITEM,
        TRAIL_STATE_3_LEFT_ODD,
        TRAIL_STATE_2_LEFT_ODD,
        TRAIL_STATE_1_LEFT_ODD,
        TRAIL_STATE_USED,
        TRAIL_STATE_3_LEFT_EVEN,
        TRAIL_STATE_2_LEFT_EVEN,
        TRAIL_STATE_1_LEFT_EVEN,
    };

    u8 recvTime;
    u8 heldItem;
    u8 trailedItem;
    u8 heldState;
    u8 trailState;
    u8 ack;
    u8 ackTimer;
    u8 pad;
};
size_assert(RKNetITEMPacket, 0x8);
