#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    char data[1400];
    int dataSize;
} qr2_buffer;
size_cassert(qr2_buffer, 0x57C);

#ifdef __cplusplus
}
#endif
