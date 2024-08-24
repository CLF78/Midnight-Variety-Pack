#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    float x, y;
} VEC2;
size_cassert(VEC2, 0x8);

typedef struct {
    float x, y, z;
} VEC3;
size_cassert(VEC3, 0xC);

typedef struct {
    float x, y, z, w;
} QUAT;
size_cassert(QUAT, 0x10);

typedef struct {
    s16 x, y, z;
} S16VEC;
size_cassert(S16VEC, 0x6);

typedef float MTX33[3][3];
typedef float MTX34[3][4];
typedef float MTX44[4][4];

#ifdef __cplusplus
}
#endif
