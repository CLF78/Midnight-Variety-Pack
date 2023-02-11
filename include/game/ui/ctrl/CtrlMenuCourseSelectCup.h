#include <kamek.h>
#include <game/ui/LayoutUIControl.h>
#include <game/ui/ctrl/CtrlMenuCourseSelectCupSub.h>

class CtrlMenuCourseSelectCup : public LayoutUIControl {
public:
    virtual ~CtrlMenuCourseSelectCup();

    virtual void initSelf();

    virtual const TypeInfo* getTypeInfo() const;
    virtual const char* getTypeName() const;

    CtrlMenuCourseSelectCupSub cups[8];
};
size_assert(CtrlMenuCourseSelectCup, 0xDD4);
