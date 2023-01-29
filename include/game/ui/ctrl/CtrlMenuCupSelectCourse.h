#include <kamek.h>
#include <game/ui/LayoutUIControl.h>

class CtrlMenuCupSelectCourse : public LayoutUIControl {
public:
    virtual ~CtrlMenuCupSelectCourse();

    virtual void initSelf();
    virtual void calcSelf();

    virtual const TypeInfo* getTypeInfo() const;
    virtual const char* getTypeName() const;

    LayoutUIControl courseNames[4];
};
size_assert(CtrlMenuCupSelectCourse, 0x744);
