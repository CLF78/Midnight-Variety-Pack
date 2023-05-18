#include <kamek.h>
#include <game/system/SaveGhostManager.h>
#include <game/ui/GlobalContext.h>
#include <game/ui/MessageGroup.h>
#include <game/ui/Section.h>

class SectionManager {
public:
    static u8 getPlayerCount();

    Section* curSection;
    u8 unk[0x8C];

    SaveGhostManager* saveGhostManager;
    MessageGroup* systemMsgGroup;
    GlobalContext* globalContext;

    static SectionManager* instance;
};
size_assert(SectionManager, 0x9C);
