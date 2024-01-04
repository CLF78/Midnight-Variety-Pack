#include <common/Common.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    u32 flags;
    u32 reserved[2];
} DWCAccFlag;

typedef struct {
    u32 userId[2];
    u32 playerId;
} DWCLoginId;

typedef struct {
    u32 id_data;
    u32 friendkey[2];
} DWCFriendKey;

typedef struct {
    u32 id_data;
    int id;
    u32 reserved;
} DWCGsProfileId;

typedef union {
    DWCAccFlag flags;
    DWCLoginId login_id;
    DWCFriendKey friend_key;
    DWCGsProfileId gs_profile_id;
} DWCFriendData;

typedef struct {
    int size;
    DWCLoginId pseudo;
    DWCLoginId authentic;
    int gs_profile_id;
    int flag;
    u32 gamecode;
    int reserved[5];
    u32 crc32;
} DWCUserData;

#ifdef __cplusplus
}
#endif
