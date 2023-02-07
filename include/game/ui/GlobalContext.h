#include <kamek.h>

class GlobalContext {
public:
    u8 unk[0x14C];
    s32 lastCourse;
    s32 lastStage;
    u8 unk2[0x3BC];
};
size_assert(GlobalContext, 0x510);
