#pragma once
#include "gp.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct GPICallbackData GPICallbackData;

typedef struct {
    GPCallback callback;
    void* param;
} GPICallback;
size_cassert(GPICallback, 0x8);

struct GPICallbackData {
    GPICallback callback;
    void* arg;
    int type;
    int operationID;
    GPICallbackData* next;
};
size_cassert(GPICallbackData, 0x18);

#ifdef __cplusplus
}
#endif
