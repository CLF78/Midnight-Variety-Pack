#include <common/Common.h>

#ifdef __cplusplus
extern "C" {
#endif

enum {
    WPAD_CONTROLLER_1,
    WPAD_CONTROLLER_2,
    WPAD_CONTROLLER_3,
    WPAD_CONTROLLER_4,
    WPAD_CONTROLLER_COUNT,
};

enum {
    WPAD_BUTTON_LEFT = 0x0001,
    WPAD_BUTTON_RIGHT = 0x0002,
    WPAD_BUTTON_DOWN = 0x0004,
    WPAD_BUTTON_UP = 0x0008,
    WPAD_BUTTON_PLUS = 0x0010,
    WPAD_BUTTON_2 = 0x0100,
    WPAD_BUTTON_1 = 0x0200,
    WPAD_BUTTON_B = 0x0400,
    WPAD_BUTTON_A = 0x0800,
    WPAD_BUTTON_MINUS = 0x1000,
    WPAD_BUTTON_Z = 0x2000,
    WPAD_BUTTON_C = 0x4000,
    WPAD_BUTTON_HOME = 0x8000,
};

enum {
    WPAD_CL_BUTTON_UP = 0x0001,
    WPAD_CL_BUTTON_LEFT = 0x0002,
    WPAD_CL_TRIGGER_ZR = 0x0004,
    WPAD_CL_BUTTON_X = 0x0008,
    WPAD_CL_BUTTON_A = 0x0010,
    WPAD_CL_BUTTON_Y = 0x0020,
    WPAD_CL_BUTTON_B = 0x0040,
    WPAD_CL_TRIGGER_ZL = 0x0080,
    WPAD_CL_TRIGGER_R = 0x0200,
    WPAD_CL_BUTTON_PLUS = 0x0400,
    WPAD_CL_BUTTON_HOME = 0x0800,
    WPAD_CL_BUTTON_MINUS = 0x1000,
    WPAD_CL_TRIGGER_L = 0x2000,
    WPAD_CL_BUTTON_DOWN = 0x4000,
    WPAD_CL_BUTTON_RIGHT = 0x8000,
};

enum {
    WPAD_TYPE_WIIMOTE,
    WPAD_TYPE_NUNCHUK,
    WPAD_TYPE_CLASSIC,
};

enum {
    WPAD_ERR_NONE = 0,
    WPAD_ERR_NO_CONTROLLER = -1,
    WPAD_ERR_BUSY = -2,
    WPAD_ERR_TRANSFER = -3,
    WPAD_ERR_INVALID = -4,
    WPAD_ERR_NOPERM = -5,
    WPAD_ERR_BROKEN = -6,
    WPAD_ERR_CORRUPTED = -7,
};

// Detected pointer objects
typedef struct {
    s16 x; // 0 <= value <= 1024
    s16 y; // 0 <= value <= 768
    u16 size;
    u8 traceId;
    // 1 byte padding
} DPDObject;

typedef struct {
    u16 buttons;
    s16 accelX, accelY, accelZ; // -512 <= value <= 512
    DPDObject obj[4];

    u8 deviceType;
    s8 errorCode;
} WPADStatus;

typedef struct {
    u16 buttons;
    s16 accelX, accelY, accelZ; // -512 <= value <= 512
    DPDObject obj[4];

    u8 deviceType;
    s8 errorCode;

    s16 nunchukAccelX, nunchukAccelY, nunchukAccelZ; // -512 <= value <= 512
    s8 stickX, stickY; // -128 <= value <= 128
} WPADFSStatus;

typedef struct {
    u16 buttons;
    s16 accelX, accelY, accelZ; // -512 <= value <= 512
    DPDObject obj[4];

    u8 deviceType;
    s8 errorCode;

    u16 clButtons;
    s16 leftStickX, leftStickY; // -512 <= value <= 512
    s16 rightStickX, rightStickY; // -512 <= value <= 512
    u8 leftTrigger, rightTrigger; // always zero
} WPADCLStatus;

s32 WPADProbe(s32 chan, u32* type);

#ifdef __cplusplus
}
#endif
