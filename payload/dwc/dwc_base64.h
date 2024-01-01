#include <common/Common.h>

#ifdef __cplusplus
extern "C" {
#endif

int DWC_Base64Encode(const char* src, const ulong srclen, char* dst, const ulong dstlen);
int DWC_Base64Decode(const char* src, const ulong srclen, char* dst, const ulong dstlen);

int DWC_GetDecodedSize(const int encodedLen) {
    return (encodedLen / 4) * 3;
}

int DWC_GetEncodedSize(const int decodedLen) {
    return (decodedLen / 3) * 4;
}

#ifdef __cplusplus
}
#endif
