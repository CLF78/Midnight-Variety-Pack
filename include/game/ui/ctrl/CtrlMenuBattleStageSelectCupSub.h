#include <kamek.h>
#include <game/ui/LayoutUIControl.h>
#include <nw4r/lyt/pane.h>
#include <rvl/mtx/mtx.h>
#include <stdlib/new.h>

class CtrlMenuBattleStageSelectCupSub : public LayoutUIControl {
public:
    CtrlMenuBattleStageSelectCupSub();
    virtual ~CtrlMenuBattleStageSelectCupSub();

    virtual void initSelf();
    virtual void calcSelf();

    virtual const TypeInfo* getTypeInfo() const;
    virtual const char* getTypeName() const;

    float alpha;
    nw4r::lyt::Pane* rootPane;
    VEC3 somePos;
    bool selected;
    u8 fadeDirection; // 0 = entering, 1 = exiting

    static CtrlMenuBattleStageSelectCupSub* construct(void* buffer) {
        return new(buffer) CtrlMenuBattleStageSelectCupSub;
    }
};
size_assert(CtrlMenuBattleStageSelectCupSub, 0x18C);
