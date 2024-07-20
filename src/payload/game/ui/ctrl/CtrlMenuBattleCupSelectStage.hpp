#include <common/Common.hpp>
#include <game/ui/ctrl/LayoutUIControl.hpp>

class CtrlMenuBattleCupSelectStage : public LayoutUIControl {
public:
    virtual ~CtrlMenuBattleCupSelectStage();

    virtual void initSelf();
    virtual void calcSelf();

    RUNTIME_TYPE_INFO_NODECL;
    RUNTIME_TYPENAME_INFO_NODECL;

    void load();
    void setCourseNames(u32 cupButtonId);

    LayoutUIControl courseNames[4];
    LayoutUIControl unused;

    static const char* animNames[16];
};
size_assert(CtrlMenuBattleCupSelectStage, 0x8B8);
