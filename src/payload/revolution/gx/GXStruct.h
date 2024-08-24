#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    u8 r, g, b, a;
} GXColor;
size_cassert(GXColor, 0x4);

#ifdef __cplusplus
}
#endif
