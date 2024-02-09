#include <common/Common.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    PAD_CONTROLLER_1,
    PAD_CONTROLLER_2,
    PAD_CONTROLLER_3,
    PAD_CONTROLLER_4,
    PAD_CONTROLLER_COUNT,
} PADController;

typedef enum {
    PAD_BUTTON_LEFT  = BIT_FLAG(0),
    PAD_BUTTON_RIGHT = BIT_FLAG(1),
    PAD_BUTTON_DOWN  = BIT_FLAG(2),
    PAD_BUTTON_UP    = BIT_FLAG(3),
    PAD_TRIGGER_Z    = BIT_FLAG(4),
    PAD_TRIGGER_R    = BIT_FLAG(5),
    PAD_TRIGGER_L    = BIT_FLAG(6),
    PAD_BUTTON_A     = BIT_FLAG(8),
    PAD_BUTTON_B     = BIT_FLAG(9),
    PAD_BUTTON_X     = BIT_FLAG(10),
    PAD_BUTTON_Y     = BIT_FLAG(11),
    PAD_BUTTON_START = BIT_FLAG(12),
} PADButton;

typedef enum {
    PAD_ERR_NONE          = 0,
    PAD_ERR_NO_CONTROLLER = -1,
    PAD_ERR_NOT_READY     = -2,
    PAD_ERR_TRANSFER      = -3,
} PADErr;

typedef struct {
    u16 buttons;
    s8 stickX, stickY;            // -128 <= value <= 127
    s8 substickX, substickY;      // -128 <= value <= 127
    u8 triggerLeft, triggerRight; // 0 <= value <= 255
    u8 analogA, analogB;          // 0 <= value <= 255
    s8 error;
} PADStatus;

u32 PADRead(PADStatus* status);
void PADClampCircle(PADStatus* status);

#ifdef __cplusplus
}
#endif
