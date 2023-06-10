#include <kamek.h>
#include <game/ui/LayoutUIControl.h>
#include <game/ui/ctrl/CtrlMenuBattleStageSelectCupSub.h>

class CtrlMenuBattleStageSelectCup : public LayoutUIControl {
public:
    virtual ~CtrlMenuBattleStageSelectCup();

    virtual void initSelf();

    virtual const TypeInfo* getTypeInfo() const;
    virtual const char* getTypeName() const;

    CtrlMenuBattleStageSelectCupSub cups[2];
};
size_assert(CtrlMenuBattleStageSelectCup, 0x48C);
