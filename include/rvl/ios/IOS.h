#include <kamek.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    void* addr;
    u32 length;
} IOSIoVector;

int IOS_Ioctlv(s32 fd, u32 cmd, u32 readCount, u32 writeCount, IOSIoVector* vector);

#ifdef __cplusplus
}
#endif
