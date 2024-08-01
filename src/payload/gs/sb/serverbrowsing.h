#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
	SB_ERROR_NONE,
    SB_ERROR_SOCKET,
    SB_ERROR_DNS,
    SB_ERROR_CONNECT,
    SB_ERROR_INVALID_DATA,
    SB_ERROR_ALLOC,
    SB_ERROR_PARAM,
    SB_ERROR_DUPLICATE_UPDATE,
} SBError;

#ifdef __cplusplus
}
#endif
