#pragma once
#include "ButtonInfo.hpp"
#include "MenuInputManager.hpp"
#include <game/ui/InputHandler.hpp>
#include <game/ui/ctrl/LayoutUIControl.hpp>

class PageInputManager : public MenuInputManager {
public:
    RUNTIME_TYPE_INFO_NODECL;
    virtual ~PageInputManager();

    virtual void init(u32 playerFlags, bool isMultiPlayer);
    virtual void checkActions();
    virtual void activate(u32 hudSlotId, u32 localPlayerBitfield2, bool isMultiplayer);
    virtual void onReset();
    virtual void onActivate(bool isMultiPlayer);

    InputHandler2<LayoutUIControl, void, u32, u32>* handlers[INPUT_COUNT];
    bool isTriggered[INPUT_COUNT];
    PAD(3);
    ButtonInfo buttoninfoArray[5];
};
size_assert(PageInputManager, 0x144);
