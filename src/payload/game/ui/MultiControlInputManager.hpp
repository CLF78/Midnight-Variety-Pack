#include <common/Common.hpp>
#include <egg/core/eggList.hpp>
#include <game/ui/ControlInputManager.hpp>
#include <game/ui/InputHandler.hpp>
#include <game/ui/MenuInputManager.hpp>
#include <revolution/mtx.h>

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

    enum WrapType {
        XY_WRAP,
        Y_WRAP,
        NO_WRAP,
        LINEAR,
    };

    RUNTIME_TYPE_INFO_NODECL;
    virtual ~MultiControlInputManager();

    virtual void init(u32 playerFlags, bool isMultiplayer);

    typedef int (*calcDistanceFunc)(Player* src, Player* dest, u32 direction);
    void setDistanceFunc(int wrapType);
    void setHandler(int inputId, InputHandlerBase* inputHandler, bool unk, bool pointerOnly);

    EGG::List list;
    InputHandler0<MultiControlInputManager, void>* handlers[MenuInputManager::INPUT_COUNT];
    bool actionsSet[MenuInputManager::INPUT_COUNT];
    u8 _49[MenuInputManager::INPUT_COUNT];

    Player players[5];
    calcDistanceFunc distFunc;
};
size_assert(MultiControlInputManager, 0x224);
