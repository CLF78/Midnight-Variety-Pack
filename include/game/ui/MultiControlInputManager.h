#include <kamek.h>
#include <egg/core/eggList.h>
#include <game/ui/ControlInputManager.h>
#include <game/ui/InputHandler.h>
#include <game/ui/MenuInputManager.h>

class MultiControlInputManager : public MenuInputManager {
public:
    class Player {
    public:
        ControlInputManager* manager;
        u32 _4;

        void* vtable;
        int frames[MenuInputManager::INPUT_COUNT];
        u8 _28[MenuInputManager::INPUT_COUNT];

        u32 sequenceFlags;
        u32 sequenceFrames;
        bool isPointer;
        VEC2 pointerPos;
        u8 _50[0x5c - 0x50];
    };
    size_assert(Player, 0x5C);

    EGG::List list;
    InputHandler<MultiControlInputManager>* handlers[MenuInputManager::INPUT_COUNT];
    bool actionsSet[MenuInputManager::INPUT_COUNT];
    u8 _49[MenuInputManager::INPUT_COUNT];

    Player players[5];
    u32 _220;
};
size_assert(MultiControlInputManager, 0x224);
