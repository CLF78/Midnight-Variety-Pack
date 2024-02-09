#include <common/Common.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    WPAD_CONTROLLER_1,
    WPAD_CONTROLLER_2,
    WPAD_CONTROLLER_3,
    WPAD_CONTROLLER_4,
    WPAD_CONTROLLER_COUNT,
} WPADController;

typedef enum {
    WPAD_BUTTON_LEFT  = BIT_FLAG(0),
    WPAD_BUTTON_RIGHT = BIT_FLAG(1),
    WPAD_BUTTON_DOWN  = BIT_FLAG(2),
    WPAD_BUTTON_UP    = BIT_FLAG(3),
    WPAD_BUTTON_PLUS  = BIT_FLAG(4),
    WPAD_BUTTON_2     = BIT_FLAG(8),
    WPAD_BUTTON_1     = BIT_FLAG(9),
    WPAD_BUTTON_B     = BIT_FLAG(10),
    WPAD_BUTTON_A     = BIT_FLAG(11),
    WPAD_BUTTON_MINUS = BIT_FLAG(12),
    WPAD_BUTTON_Z     = BIT_FLAG(13),
    WPAD_BUTTON_C     = BIT_FLAG(14),
    WPAD_BUTTON_HOME  = BIT_FLAG(15),
} WPADButton;

typedef enum {
    WPAD_CL_BUTTON_UP    = BIT_FLAG(0),
    WPAD_CL_BUTTON_LEFT  = BIT_FLAG(1),
    WPAD_CL_TRIGGER_ZR   = BIT_FLAG(2),
    WPAD_CL_BUTTON_X     = BIT_FLAG(3),
    WPAD_CL_BUTTON_A     = BIT_FLAG(4),
    WPAD_CL_BUTTON_Y     = BIT_FLAG(5),
    WPAD_CL_BUTTON_B     = BIT_FLAG(6),
    WPAD_CL_TRIGGER_ZL   = BIT_FLAG(7),
    WPAD_CL_TRIGGER_R    = BIT_FLAG(9),
    WPAD_CL_BUTTON_PLUS  = BIT_FLAG(10),
    WPAD_CL_BUTTON_HOME  = BIT_FLAG(11),
    WPAD_CL_BUTTON_MINUS = BIT_FLAG(12),
    WPAD_CL_TRIGGER_L    = BIT_FLAG(13),
    WPAD_CL_BUTTON_DOWN  = BIT_FLAG(14),
    WPAD_CL_BUTTON_RIGHT = BIT_FLAG(15),
} WPADCLButton;

typedef enum {
    WPAD_TYPE_WIIMOTE,
    WPAD_TYPE_NUNCHUK,
    WPAD_TYPE_CLASSIC,
} WPADType;

typedef enum {
    WPAD_ERR_NONE          = 0,
    WPAD_ERR_NO_CONTROLLER = -1,
    WPAD_ERR_BUSY          = -2,
    WPAD_ERR_TRANSFER      = -3,
    WPAD_ERR_INVALID       = -4,
    WPAD_ERR_NOPERM        = -5,
    WPAD_ERR_BROKEN        = -6,
    WPAD_ERR_CORRUPTED     = -7,
} WPADErr;

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
    s8 stickX, stickY;                               // -128 <= value <= 128
} WPADFSStatus;

typedef struct {
    u16 buttons;
    s16 accelX, accelY, accelZ; // -512 <= value <= 512
    DPDObject obj[4];

    u8 deviceType;
    s8 errorCode;

    u16 clButtons;
    s16 leftStickX, leftStickY;   // -512 <= value <= 512
    s16 rightStickX, rightStickY; // -512 <= value <= 512
    u8 leftTrigger, rightTrigger; // always zero
} WPADCLStatus;

s32 WPADProbe(s32 chan, u32* type);

#ifdef __cplusplus
}
#endif
