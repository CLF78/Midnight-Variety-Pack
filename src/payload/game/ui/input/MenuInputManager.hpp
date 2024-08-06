#pragma once
#include <game/ui/TypeInfo.hpp>

class MenuInputManager {
public:
    enum InputType {
        FORWARD_PRESS,
        BACK_PRESS,
        START_PRESS,
        UP_PRESS,
        DOWN_PRESS,
        LEFT_PRESS,
        RIGHT_PRESS,
        UNK_7,
        UNK_8, // Home button maybe?
        INPUT_COUNT,
    };

    RUNTIME_TYPE_INFO_NODECL;
    virtual ~MenuInputManager();
    virtual void init(u32 playerFlags, bool isMultiPlayer);
    virtual void* getHolderList() const;
    virtual void checkActions();
    virtual void activate(u32 hudSlotId, u32 localPlayerBitfield2, bool isMultiplayer);
    virtual void onDeactivate(bool isMultiplayer);
    virtual void onReset();
    virtual void onActivate(bool isMultiPlayer);
    virtual void update();
    virtual bool isPointerEnabled();
    virtual bool checkPlayerPointer();

    u32 playerFlags;
    u32 playerEnabledFlags; // (1 << player) to enable inputs for player X
    bool inaccessible;
    bool isMultiPlayer;
    u8 _E;
    PAD(1);
};
size_assert(MenuInputManager, 0x10);
