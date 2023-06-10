#include <kamek.h>
#include <game/ui/LayoutUIControl.h>

class CtrlMenuBattleCupSelectStage : public LayoutUIControl {
public:
    virtual ~CtrlMenuBattleCupSelectStage();

    virtual void initSelf();
    virtual void calcSelf();

    virtual const TypeInfo* getTypeInfo() const;
    virtual const char* getTypeName() const;

    LayoutUIControl courseNames[5];
};
size_assert(CtrlMenuBattleCupSelectStage, 0x8B8);
