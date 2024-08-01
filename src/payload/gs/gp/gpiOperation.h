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

typedef struct {
    char serverChallenge[128];
    char userChallenge[33];
    char passwordHash[33];
    char authtoken[256];
    char partnerchallenge[256];
    char cdkey[65];
    BOOL newuser;
} GPIConnectData;

#ifdef __cplusplus
}
#endif
