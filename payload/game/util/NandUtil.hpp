#include <common/Common.hpp>

class NandUtil {
public:

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
};
