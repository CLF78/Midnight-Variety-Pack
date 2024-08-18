#pragma once

#ifdef __cplusplus
extern "C" {
#endif

NORETURN void OSFatal(unsigned int* fg, unsigned int* bg, const char* str);

#ifdef __cplusplus
}
#endif
