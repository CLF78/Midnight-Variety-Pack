#include <kamek.h>

class CupManager {
public:
    CupManager();

    // Gets the column-based index from a row-based index cup ID - algorithm by Ismy
    static inline u32 getCupIdx(u32 cupId) {
        return (cupId >> 1) + ((cupId % 2) << 2);
    }

    static CupManager* construct() { return new CupManager; }

    u16 curPage;
    u16 maxPage;

    static CupManager* instance;
};
