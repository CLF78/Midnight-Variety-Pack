#pragma once
#include "CtrlMenuMovieButton.hpp"
#include "LayoutUIControl.hpp"

class CtrlMenuBattleStageSelectStage : public LayoutUIControl {
public:
    virtual ~CtrlMenuBattleStageSelectStage();

    virtual void initSelf();

    RUNTIME_TYPE_INFO_NODECL;
    RUNTIME_TYPENAME_INFO_NODECL;

    void load(u32 playerFlags, bool unk);

    void onButtonClick(PushButton* btn, u32 hudSlotId);
    void onButtonSelect(PushButton* btn, u32 hudSlotId);

    CtrlMenuMovieButton courseButtons[4];
    CtrlMenuMovieButton unusedButton;
    LayoutUIControl _D90;
    InputHandler2<PushButton, void, PushButton*, u32> onButtonClickHandler;
    InputHandler2<PushButton, void, PushButton*, u32> onButtonSelectHandler;

    static const char* buttonAnims[6];
};
size_assert(CtrlMenuBattleStageSelectStage, 0xF2C);

