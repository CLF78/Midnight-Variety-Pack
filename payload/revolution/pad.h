#include <common/Common.h>

#ifdef __cplusplus
extern "C" {
#endif

enum {
    PAD_CONTROLLER_1,
    PAD_CONTROLLER_2,
    PAD_CONTROLLER_3,
    PAD_CONTROLLER_4,
    PAD_CONTROLLER_COUNT,
};

enum {
    PAD_BUTTON_LEFT = 0x0001,
    PAD_BUTTON_RIGHT = 0x0002,
    PAD_BUTTON_DOWN = 0x0004,
    PAD_BUTTON_UP = 0x0008,
    PAD_TRIGGER_Z = 0x0010,
    PAD_TRIGGER_R = 0x0020,
    PAD_TRIGGER_L = 0x0040,
    PAD_BUTTON_A = 0x0100,
    PAD_BUTTON_B = 0x0200,
    PAD_BUTTON_X = 0x0400,
    PAD_BUTTON_Y = 0x0800,
    PAD_BUTTON_START = 0x1000,
};

enum {
    PAD_ERR_NONE = 0,
    PAD_ERR_NO_CONTROLLER = -1,
    PAD_ERR_NOT_READY = -2,
    PAD_ERR_TRANSFER = -3,
};

typedef struct {
    u16 buttons;
    s8 stickX, stickY; // -128 <= value <= 127
    s8 substickX, substickY; // -128 <= value <= 127
    u8 triggerLeft, triggerRight; // 0 <= value <= 255
    u8 analogA, analogB; // 0 <= value <= 255
    s8 error;
} PADStatus;

u32 PADRead(PADStatus* status);
void PADClampCircle(PADStatus* status);

#ifdef __cplusplus
}
#endif
