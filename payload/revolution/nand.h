#include <common/Common.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    NAND_ACCESS_READ = BIT_FLAG(0),
    NAND_ACCESS_WRITE = BIT_FLAG(1),
    NAND_ACCESS_RW = NAND_ACCESS_READ | NAND_ACCESS_WRITE,
} NANDAccess;

typedef struct {
    int fileDescriptor;
    int originalFd;
    char originalPath[64];
    char tmpPath[64];
    u8 accessType;
    u8 stage;
    u8 mark;
    u8 pad;
} NANDFileInfo;

#ifdef __cplusplus
}
#endif
