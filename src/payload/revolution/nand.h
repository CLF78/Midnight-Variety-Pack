#pragma once
#include "os/OS.h"

#ifdef __cplusplus
extern "C" {
#endif

#define NANDBytesToBlocks(bytes) (OSRoundUp((bytes), 0x4000) / 0x4000)

typedef enum {
    NAND_ACCESS_READ = BIT_FLAG(0),
    NAND_ACCESS_WRITE = BIT_FLAG(1),
    NAND_ACCESS_RW = NAND_ACCESS_READ | NAND_ACCESS_WRITE,
} NANDAccess;

typedef enum {
    NAND_CHECK_HOME_NO_SPACE  = BIT_FLAG(0),
    NAND_CHECK_HOME_NO_INODES = BIT_FLAG(1),
    NAND_CHECK_SYS_NO_SPACE   = BIT_FLAG(2),
    NAND_CHECK_SYS_NO_INODES  = BIT_FLAG(3),
} NANDCheckResult;

typedef enum {
    NAND_PERM_OTHER_READ  = BIT_FLAG(0),
    NAND_PERM_OTHER_WRITE = BIT_FLAG(1),
    NAND_PERM_OTHER_RW    = NAND_PERM_OTHER_READ | NAND_PERM_OTHER_WRITE,
    NAND_PERM_GROUP_READ  = BIT_FLAG(2),
    NAND_PERM_GROUP_WRITE = BIT_FLAG(3),
    NAND_PERM_GROUP_RW    = NAND_PERM_GROUP_READ | NAND_PERM_GROUP_WRITE,
    NAND_PERM_OWNER_READ  = BIT_FLAG(4),
    NAND_PERM_OWNER_WRITE = BIT_FLAG(5),
    NAND_PERM_OWNER_RW    = NAND_PERM_OWNER_READ | NAND_PERM_OWNER_WRITE,
} NANDPerm;

typedef struct {
    int fileDescriptor;
    int originalFd;
    char originalPath[64];
    char tmpPath[64];
    u8 accessType;
    u8 stage;
    u8 mark;
    PAD(1);
} NANDFileInfo;
size_cassert(NANDFileInfo, 0x8C);

#ifdef __cplusplus
}
#endif
