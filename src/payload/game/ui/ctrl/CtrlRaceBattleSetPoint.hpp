#include <common/Common.hpp>
#include <game/ui/ctrl/LayoutUIControl.hpp>

class CtrlRaceBattleSetPoint : public LayoutUIControl {
public:
    CtrlRaceBattleSetPoint() {}
    virtual ~CtrlRaceBattleSetPoint();

    virtual void initSelf();

    RUNTIME_TYPE_INFO_NODECL;
    RUNTIME_TYPENAME_INFO_NODECL;

    void load(bool isBlue, int bubbleIdx);

    u32 isBlue;
    u32 _178; // unused?
    u32 bubbleIdx;
};
size_assert(CtrlRaceBattleSetPoint, 0x180);
