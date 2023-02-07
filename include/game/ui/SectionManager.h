#include <kamek.h>
#include <game/ui/GlobalContext.h>
#include <game/ui/Section.h>

class SectionManager {
public:
    static u8 getPlayerCount();

    Section* curSection;
    u8 unk[0x94];
    GlobalContext* globalContext;

    static SectionManager* instance;
};
size_assert(SectionManager, 0x9C);
