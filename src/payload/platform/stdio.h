#include <platform/stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

int sprintf(char* s, const char* format, ...);
int snprintf(char* s, size_t n, const char* format, ...);

int vsprintf(char* s, const char* format, va_list arg);
int vsnprintf(char* s, size_t n, const char* format, va_list arg);

#ifdef __cplusplus
}
#endif
