#pragma once
#include <common/Common.hpp>
#include <game/ui/ctrl/CtrlMenuCupSelectCourse.hpp>
#include <game/ui/ctrl/CtrlMenuMovieLayout.hpp>
#include <game/ui/InputHandler.hpp>
#include <game/ui/ctrl/LayoutUIControl.hpp>
#include <game/ui/ctrl/PushButton.hpp>

class CtrlMenuCupSelectCup : public LayoutUIControl {
public:
    virtual ~CtrlMenuCupSelectCup();
    virtual void initSelf();

    RUNTIME_TYPE_INFO_NODECL;
    RUNTIME_TYPENAME_INFO_NODECL;

    void load(u32 playerFlags, bool unk);

    void onButtonClick(PushButton* btn, u32 hudSlotId);
    void onButtonSelect(PushButton* btn, u32 hudSlotId);

    PushButton cupButtons[8];
    InputHandler2<PushButton, void, PushButton*, u32> onButtonClickHandler;
    InputHandler2<PushButton, void, PushButton*, u32> onButtonSelectHandler;
    u32 currentSelected;
    CtrlMenuMovieLayout movieLayout;
};
size_assert(CtrlMenuCupSelectCup, 0x15CC);
