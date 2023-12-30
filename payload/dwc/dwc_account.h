#include <common/Common.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    u32 userId[2]; // actually u64, but CW is stupid and adds padding when there isn't any
    u32 playerId;
} DWCAccLoginId;

typedef struct {
    int size;
    DWCAccLoginId pseudo;
    DWCAccLoginId authentic;
    int gs_profile_id;
    int flag;
    u32 gamecode;
    int reserved[5];
    u32 crc32;
} DWCUserData;

#ifdef __cplusplus
}
#endif
