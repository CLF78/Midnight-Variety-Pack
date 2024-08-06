#pragma once
#include "gp.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    char* buffer;
    int size;
    int len;
    int pos;
} GPIBuffer;
size_cassert(GPIBuffer, 0x10);

GPResult gpiAppendStringToBuffer(GPConnection connection, GPIBuffer* outputBuffer, const char* buffer);

#ifdef __cplusplus
}
#endif
