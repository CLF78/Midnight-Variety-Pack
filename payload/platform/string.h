#include <common/Common.h>

#ifdef __cplusplus
extern "C" {
#endif

void* memcpy(void* destination, const void* source, size_t size);
void* memset(void* ptr, int value, size_t num);

int strcmp(const char* str1, const char* str2);
int strncmp(const char* str1, const char* str2, size_t num);
char* strncpy(char* destination, const char* source, size_t num);
char* strstr(char* str1, const char* str2);
size_t strlen(const char* str);

#ifdef __cplusplus
}
#endif
