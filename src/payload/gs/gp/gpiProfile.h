#pragma once
#include <gs/hashtable.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    HashTable profileHashTable;
    int num;
    int numBuddies;
} GPIProfileList;
size_cassert(GPIProfileList, 0xC);

#ifdef __cplusplus
}
#endif
