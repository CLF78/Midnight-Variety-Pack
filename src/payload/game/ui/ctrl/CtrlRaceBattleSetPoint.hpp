#include <common/Common.hpp>
#include <game/ui/LayoutUIControl.hpp>

class CtrlRaceBattleSetPoint : public LayoutUIControl {
public:
    CtrlRaceBattleSetPoint() {}
    virtual ~CtrlRaceBattleSetPoint();

    virtual void initSelf();

    virtual const TypeInfo* getTypeInfo() const;
    virtual const char* getTypeName() const;

    void load(bool isBlue, int bubbleIdx);

    u32 isBlue;
    u32 _178; // unused?
    u32 bubbleIdx;
};
size_assert(CtrlRaceBattleSetPoint, 0x180);
