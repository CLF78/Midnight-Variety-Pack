#include <kamek.h>

#ifdef __cplusplus
extern "C" {
#endif

void* operator new(size_t size, void* buffer) {
    return buffer;
}

#ifdef __cplusplus
}
#endif
