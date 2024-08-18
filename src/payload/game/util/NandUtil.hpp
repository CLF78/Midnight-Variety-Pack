#pragma once
#include <revolution/nand.h>

namespace NandUtil {

    enum Result {
        ERROR_NONE,
        ERROR_UNUSED,
        ERROR_BUSY,
        ERROR_FILE_CORRUPT,
        ERROR_FILE_NOT_FOUND,
        ERROR_BANNER,
        ERROR_NAND_CORRUPT,
        ERROR_SPACE,
        ERROR_OTHER,
        ERROR_REGION,
        ERROR_GHOST_SOMETHING,
    };

    enum CheckResultFlag {
        CHECK_ERROR_NONE =   BIT_FLAG(-1),
        CHECK_ERROR_FAIL =   BIT_FLAG(0), // NANDCheck returned an error
        CHECK_ERROR_BLOCKS = BIT_FLAG(1), // Not enough blocks available in title directory
        CHECK_ERROR_INODES = BIT_FLAG(2), // Too many files in title directory
    };

    enum Type {
        TYPE_NONE,
        TYPE_FILE,
        TYPE_DIR,
    };

    int Check(u32 requiredBlocks, u32 requiredInodes, u32* answer);
    int Create(const char* path, u8 perms);
    int Open(const char* path, NANDFileInfo* info, int access);
    int GetType(const char* path, u32* type);
    int GetLength(NANDFileInfo* info, u32* length);
    int Read(NANDFileInfo* info, void* buffer, u32 size, int offset);
    int Write(NANDFileInfo* info, void* buffer, u32 size, int offset);
    int Close(NANDFileInfo* info);
    int Delete(const char* path);

} // namespace NandUtil
