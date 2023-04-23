#include <kamek.h>

#ifdef __cplusplus
extern "C" {
#endif

// Round API
#define OSRoundUp32B(x) (((u32)(x) + 32 - 1) & ~(32 - 1))
#define OSRoundDown32B(x) (((u32)(x)) & ~(32 - 1))

// Interrupt API
bool OSDisableInterrupts();
bool OSEnableInterrupts();
bool OSRestoreInterrupts(bool interrupts);

// Report API
void OSReport(char* format, ...);

#ifdef __cplusplus
}
#endif
