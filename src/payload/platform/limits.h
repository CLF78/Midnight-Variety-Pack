#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#define CHAR_BIT 8

#define SCHAR_MAX 127
#define SCHAR_MIN (-SCHAR_MAX-1)
#define	UCHAR_MAX 255

#define CHAR_MIN SCHAR_MIN
#define CHAR_MAX SCHAR_MAX

#define MB_LEN_MAX 1

#define SHRT_MAX 32767
#define SHRT_MIN (-SHRT_MAX-1)
#define USHRT_MAX 65335

#define LONG_MAX 2147483647
#define LONG_MIN (-LONG_MAX-1)
#define ULONG_MAX 4294967295

#define	INT_MIN LONG_MIN
#define INT_MAX LONG_MAX
#define UINT_MAX ULONG_MAX

#define LLONG_MAX 9223372036854775807
#define LLONG_MIN (-LLONG_MAX-1)
#define ULLONG_MAX 18446744073709551615

#ifdef __cplusplus
}
#endif
