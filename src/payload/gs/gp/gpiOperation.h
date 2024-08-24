#pragma once
#include "gpiCallback.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct GPIOperation GPIOperation;

struct GPIOperation {
    int type;
    void* data;
    int blocking;
    GPICallback callback;
    int state;
    int id;
    int result;
    GPIOperation* next;
};
size_cassert(GPIOperation, 0x24);

typedef struct {
    char serverChallenge[128];
    char userChallenge[33];
    char passwordHash[33];
    char authtoken[256];
    char partnerchallenge[256];
    char cdkey[65];
    PAD(1);
    BOOL newuser;
} GPIConnectData;
size_cassert(GPIConnectData, 0x308);

#ifdef __cplusplus
}
#endif
