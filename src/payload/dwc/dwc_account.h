#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#define DWC_ACC_MASKBITS(bits) (((1 << (bits)) - 1))
#define DWC_ACC_FLAGS_SHIFT 11
#define DWC_ACC_FLAGS_BITS 21
#define DWC_ACC_FLAGS_MASK DWC_ACC_MASKBITS(DWC_ACC_FLAGS_BITS)

typedef struct {
    u32 flags;
    PAD(8);
} DWCAccFlag;
size_cassert(DWCAccFlag, 0xC);

typedef struct {
    u32 id_data;
    u32 userId;
    u32 playerId;
} DWCLoginId;
size_cassert(DWCLoginId, 0xC);

typedef struct {
    u32 id_data;
    u32 friendkey[2];
} DWCFriendKey;
size_cassert(DWCFriendKey, 0xC);

typedef struct {
    u32 id_data;
    int id;
    PAD(4);
} DWCGsProfileId;
size_cassert(DWCGsProfileId, 0xC);

typedef union {
    DWCAccFlag flags;
    DWCLoginId login_id;
    DWCFriendKey friend_key;
    DWCGsProfileId gs_profile_id;
} DWCFriendData;
size_cassert(DWCFriendData, 0xC);

typedef enum {
    DWC_FRIENDDATA_NODATA,
    DWC_FRIENDDATA_LOGIN_ID,
    DWC_FRIENDDATA_FRIEND_KEY,
    DWC_FRIENDDATA_GS_PROFILE_ID,
    DWC_FRIENDDATA_MASK = 3,
} DWCFriendDataType;

typedef struct {
    int size;
    DWCLoginId pseudo;
    DWCLoginId authentic;
    int gs_profile_id;
    int flag;
    u32 gamecode;
    PAD(20);
    u32 crc32;
} DWCUserData;
size_cassert(DWCUserData, 0x40);

typedef enum {
    DWC_USER_DATA_FLAG_DIRTY = BIT_FLAG(0),
} DWCUserDataFlag;

void DWCi_Acc_SetUserId(DWCLoginId* data, u64 userid);

#ifdef __cplusplus
}
#endif
