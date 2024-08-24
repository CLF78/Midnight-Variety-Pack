#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*ArrayFreeFunc)(void* element);

typedef struct {
    int count;
    int capacity;
    int elemSize;
    int growBy;
    ArrayFreeFunc elemFreeFunc;
    void* list;
} DArrayImplementation;
size_cassert(DArrayImplementation, 0x18);

typedef DArrayImplementation* DArray;

#ifdef __cplusplus
}
#endif
