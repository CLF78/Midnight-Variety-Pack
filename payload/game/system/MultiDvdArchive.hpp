#include <common/Common.hpp>
#include <game/system/DvdArchive.hpp>

class MultiDvdArchive {
public:
    enum Type {
        COMMON,
        COURSE,
        MENU,
        FONT,
        EARTH,
        MIIBODY,
        DRIVER,
        DEMO,
        BACKMODEL
    };

    enum KindFlag {
        SUFFIX_ONLY = 0x0,
        FULL_NAME = 0x1,
        CACHED = 0x4
    };

    virtual ~MultiDvdArchive();
    virtual void init();

    bool isLoaded();
    bool exists(const char* path);

    DvdArchive* archives;
    u16 archiveCount;
    u32* fileSizes;
    char** suffixes;
    void** fileStarts;
    int* kinds;

    static MultiDvdArchive* create(int type);
};
size_assert(MultiDvdArchive, 0x1C);
