#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    u8* buffer;
    int size;
    int len;
} GTI2Buffer;
size_cassert(GTI2Buffer, 0xC);

#ifdef __cplusplus
}
#endif
