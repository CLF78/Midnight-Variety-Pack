#pragma once
#include "ControlInputManager.hpp"
#include "MenuInputManager.hpp"
#include <egg/core/eggList.hpp>
#include <game/ui/InputHandler.hpp>
#include <game/ui/page/Page.hpp>
#include <revolution/mtx.h>

class MultiControlInputManager : public MenuInputManager {
public:

    class Player {
    public:
        ControlInputManager* manager;
        u32 _4;

        void* vtable;
        int frames[INPUT_COUNT];
        u8 _28[INPUT_COUNT];
        PAD(3);

        u32 sequenceFlags;
        u32 sequenceFrames;
        bool isPointer;
        PAD(3);

        VEC2 pointerPos;
        bool enabled;
        UNK(0x5C - 0x51);
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
    virtual void* getHolderList() const;
    virtual void checkActions();
    virtual void activate(u32 hudSlotId, u32 localPlayerBitfield2, bool isMultiplayer);
    virtual void onDeactivate(bool isMultiplayer);
    virtual void onReset();
    virtual void onActivate(bool isMultiPlayer);
    virtual void update();
    virtual bool isPointerEnabled();
    virtual bool checkPlayerPointer();

    typedef int (*calcDistanceFunc)(Player* src, Player* dest, u32 direction);
    void setDistanceFunc(int wrapType);
    void setHandler(int inputId, InputHandlerBase* inputHandler, bool unk, bool pointerOnly);

    EGG::List list;
    InputHandler1<Page, void, u32>* handlers[INPUT_COUNT];
    bool actionsSet[INPUT_COUNT];
    bool pointerEnabled[INPUT_COUNT];
    PAD(2);

    Player players[5];
    calcDistanceFunc distFunc;
};
size_assert(MultiControlInputManager, 0x224);
