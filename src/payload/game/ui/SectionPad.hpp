#pragma once

struct PadInfo {
    enum Status {
        STATUS_CONNECTED,
        STATUS_WAITING_FOR_INPUT,
        STATUS_DISCONNECTED,
        STATUS_INVALID,
    };

    u32 controllerId;
    u32 controllerIdActive;
    Status status;
    void* controllerHolder;
};
size_assert(PadInfo, 0x10);

class SectionPad {
public:
    virtual ~SectionPad();

    void resetStatus(bool isNotRaceScene);
    void setControllers();

    PadInfo padInfos[5]; // 5th player is dummy
    bool isNotRaceScene;
    // 3 bytes padding
    u32 localPlayerBitfield;
};
size_assert(SectionPad, 0x5C);
