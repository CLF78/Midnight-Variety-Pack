#include <common/Common.hpp>
#include <game/ui/ctrl/LayoutUIControl.hpp>
#include <game/ui/ctrl/CtrlMenuBattleStageSelectCupSub.hpp>

class CtrlMenuBattleStageSelectCup : public LayoutUIControl {
public:
    virtual ~CtrlMenuBattleStageSelectCup();

    virtual void initSelf();

    RUNTIME_TYPE_INFO_NODECL;
    RUNTIME_TYPENAME_INFO_NODECL;

    void load();

    CtrlMenuBattleStageSelectCupSub cups[2];

    static const char* cupAnimNames[16];
};
size_assert(CtrlMenuBattleStageSelectCup, 0x48C);
