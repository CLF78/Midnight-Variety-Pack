#include <kamek.h>
#include <egg/core/eggHeap.h>

class DvdArchive {
public:

    enum State {
        CLEARED,
        UNK_1,
        RIPPED,
        DECOMPRESSED,
        MOUNTED,
    };

    virtual ~DvdArchive();

    void* archive; // actually EGG::Archive
    void* archiveBuffer;
    u32 archiveSize;
    EGG::Heap* archiveHeap;
    void* fileBuffer;
    u32 fileSize;
    EGG::Heap* fileHeap;
    u32 state;
};
size_assert(DvdArchive, 0x24);
