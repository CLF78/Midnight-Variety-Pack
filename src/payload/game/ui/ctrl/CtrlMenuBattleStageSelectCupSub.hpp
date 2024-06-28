#include <common/Common.hpp>
#include <game/ui/LayoutUIControl.hpp>
#include <nw4r/lyt/pane.hpp>
#include <revolution/mtx.h>

class CtrlMenuBattleStageSelectCupSub : public LayoutUIControl {
public:
    CtrlMenuBattleStageSelectCupSub();
    virtual ~CtrlMenuBattleStageSelectCupSub();

    virtual void initSelf();
    virtual void calcSelf();

    virtual const TypeInfo* getTypeInfo() const;
    virtual const char* getTypeName() const;

    virtual void vf_3C(VEC3* vector);

    float alpha;
    nw4r::lyt::Pane* rootPane;
    VEC3 somePos;
    bool selected;
    u8 fadeDirection; // 0 = entering, 1 = exiting
};
size_assert(CtrlMenuBattleStageSelectCupSub, 0x18C);
