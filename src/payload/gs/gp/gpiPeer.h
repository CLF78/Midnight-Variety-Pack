#pragma once
#include "gpiBuffer.h"
#include <gs/darray.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct GPIPeer GPIPeer;

struct GPIPeer {
    int state;
    int initiated;
    int socket;
    int profile;
    int timeout;
    int nackCount;
    GPIBuffer inputBuffer;
    GPIBuffer outputBuffer;
    DArray messages;
    GPIPeer* next;
};
size_cassert(GPIPeer, 0x40);

#ifdef __cplusplus
}
#endif
