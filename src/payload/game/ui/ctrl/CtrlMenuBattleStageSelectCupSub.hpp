#pragma once
#include "LayoutUIControl.hpp"
#include <nw4r/lyt/pane.hpp>
#include <revolution/mtx.h>

class CtrlMenuBattleStageSelectCupSub : public LayoutUIControl {
public:
    CtrlMenuBattleStageSelectCupSub();
    virtual ~CtrlMenuBattleStageSelectCupSub();

    virtual void initSelf();
    virtual void calcSelf();

    RUNTIME_TYPE_INFO_NODECL;
    RUNTIME_TYPENAME_INFO_NODECL;

    virtual void vf_3C(VEC3* vector);

    float alpha;
    nw4r::lyt::Pane* rootPane;
    VEC3 somePos;
    bool selected;
    u8 fadeDirection; // 0 = entering, 1 = exiting
    PAD(2);
};
size_assert(CtrlMenuBattleStageSelectCupSub, 0x18C);
