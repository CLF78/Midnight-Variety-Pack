#pragma once
#include "CtrlMenuMovieLayout.hpp"
#include "LayoutUIControl.hpp"
#include "PushButton.hpp"
#include <game/ui/InputHandler.hpp>

class CtrlMenuCupSelectCup : public LayoutUIControl {
public:
    virtual ~CtrlMenuCupSelectCup();
    virtual void initSelf();

    RUNTIME_TYPE_INFO_NODECL;
    RUNTIME_TYPENAME_INFO_NODECL;

    void load(u32 playerFlags, bool unk);

    void onButtonClick(PushButton* button, u32 hudSlotId);
    void onButtonSelect(PushButton* button, u32 hudSlotId);

    PushButton cupButtons[8];
    InputHandler2<PushButton, void, PushButton*, u32> onButtonClickHandler;
    InputHandler2<PushButton, void, PushButton*, u32> onButtonSelectHandler;
    u32 currentSelected;
    CtrlMenuMovieLayout movieLayout;
};
size_assert(CtrlMenuCupSelectCup, 0x15CC);
