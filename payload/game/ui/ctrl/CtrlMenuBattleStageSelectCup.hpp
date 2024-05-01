#include <common/Common.hpp>
#include <game/ui/LayoutUIControl.hpp>
#include <game/ui/ctrl/CtrlMenuBattleStageSelectCupSub.hpp>

class CtrlMenuBattleStageSelectCup : public LayoutUIControl {
public:
    virtual ~CtrlMenuBattleStageSelectCup();

    virtual void initSelf();

    virtual const TypeInfo* getTypeInfo() const;
    virtual const char* getTypeName() const;

    void load();

    CtrlMenuBattleStageSelectCupSub cups[2];

    static const char* cupAnimNames[16];
};
size_assert(CtrlMenuBattleStageSelectCup, 0x48C);
