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
        BACKMODEL,
    };

    enum KindFlag {
        SUFFIX_ONLY = BIT_FLAG(-1),
        FULL_NAME   = BIT_FLAG(0),
        CACHED      = BIT_FLAG(2),
    };

    MultiDvdArchive(u16 archiveCount = 1);
    virtual ~MultiDvdArchive();
    virtual void init();

    bool isLoaded();
    bool exists(const char* path);
    void* getFile(const char* path, u32* size);

    DvdArchive* archives;
    u16 archiveCount;
    u32* fileSizes;
    char** suffixes;
    void** fileStarts;
    int* kinds;

    static MultiDvdArchive* create(int type);
};
size_assert(MultiDvdArchive, 0x1C);


class CourseMultiDvdArchive : public MultiDvdArchive {
public:
    CourseMultiDvdArchive() : MultiDvdArchive(4) { init(); }
    virtual ~CourseMultiDvdArchive();
    virtual void init();
};
size_assert(CourseMultiDvdArchive, sizeof(MultiDvdArchive));


class MenuMultiDvdArchive : public MultiDvdArchive {
public:
    MenuMultiDvdArchive();
    virtual ~MenuMultiDvdArchive();
    virtual void init();
};
size_assert(MenuMultiDvdArchive, sizeof(MultiDvdArchive));


class RaceMultiDvdArchive : public MultiDvdArchive {
public:
    RaceMultiDvdArchive();
    virtual ~RaceMultiDvdArchive();
    virtual void init();
};
size_assert(RaceMultiDvdArchive, sizeof(MultiDvdArchive));
