#include <common/Common.hpp>
#include <game/ui/LayoutUIControl.hpp>

class CtrlMenuBattleCupSelectStage : public LayoutUIControl {
public:
    virtual ~CtrlMenuBattleCupSelectStage();

    virtual void initSelf();
    virtual void calcSelf();

    virtual const TypeInfo* getTypeInfo() const;
    virtual const char* getTypeName() const;

    LayoutUIControl courseNames[4];
    LayoutUIControl unused;

    static const char* animNames[16];
};
size_assert(CtrlMenuBattleCupSelectStage, 0x8B8);
