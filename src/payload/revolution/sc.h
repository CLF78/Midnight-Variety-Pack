#pragma once
#include <common/Common.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    SC_LANG_JAPANESE,
    SC_LANG_ENGLISH,
    SC_LANG_GERMAN,
    SC_LANG_FRENCH,
    SC_LANG_SPANISH,
    SC_LANG_ITALIAN,
    SC_LANG_DUTCH,
    SC_LANG_SIMP_CHINESE,
    SC_LANG_TRAD_CHINESE,
    SC_LANG_KOREAN,
} SCLanguage;

typedef struct {
    u32 id;
    u16 countryName[16][64];
    u16 regionName[16][64];
    u16 latitude;
    u16 longitude;
} SCSimpleAddressData;

u8 SCGetLanguage();
const char *SCGetProductCode();

#ifdef __cplusplus
}
#endif
