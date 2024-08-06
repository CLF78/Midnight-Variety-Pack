#pragma once
#include "mtx.h"
#include "wpad.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    KPAD_READ_ERR_NONE          = 0,
    KPAD_READ_ERR_NO_DATA       = -1,
    KPAD_READ_ERR_NO_CONTROLLER = -2,
    KPAD_READ_ERR_SETUP         = -3,
    KPAD_READ_ERR_LOCKED        = -4,
    KPAD_READ_ERR_INIT          = -5,
} KPADError;

typedef union {
    struct {
        VEC2 stick;
        VEC3 accel;
        float accelSpeed;
        float accelDelta;
    } nunchuk;

    struct {
        u32 buttonsHeld;
        u32 buttonsTriggered;
        u32 buttonsReleased;

        VEC2 leftStick;
        VEC2 rightStick;

        float leftTrigger; // always 0
        float rightTrigger; // always 0
    } classic;
} KPADEXStatus;
size_cassert(KPADEXStatus, 0x24); // ignoring Balance Board stuff

typedef struct {
    u32 buttonsHeld;
    u32 buttonsTriggered;
    u32 buttonsReleased;

    VEC3 accel;
    float accelSpeed;
    float accelDelta;

    VEC2 pointerPos;
    VEC2 prevPointerPos;
    float pointerDelta;

    VEC2 pointerHorizon;
    VEC2 prevPointerHorizon;
    float pointerHorizonDelta;

    float pointerDistance;
    float prevPointerDistance;
    float pointerDistanceDelta;

    VEC2 orientation;

    u8 controllerType;
    s8 wpadError;
    s8 pointerDataReliability;
    u8 dataFormat;

    KPADEXStatus exStatus;
} KPADStatus;
size_cassert(KPADStatus, 0x84);

typedef struct {
    union {
        WPADStatus core;
        WPADFSStatus fs;
        WPADCLStatus cl;
    };
    u8 fmt;
    PAD(3);
} KPADUnifiedWpadStatus;
size_cassert(KPADUnifiedWpadStatus, 0x3C);

s32 KPADRead(s32 controller, KPADStatus samplingBufs[], u32 controllerCount);

void KPADGetUnifiedWpadStatus(s32 controller, KPADUnifiedWpadStatus* dst, u32 controllerCount);

#ifdef __cplusplus
}
#endif
