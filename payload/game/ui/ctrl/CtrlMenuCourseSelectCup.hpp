#include <common/Common.hpp>
#include <game/ui/LayoutUIControl.hpp>
#include <game/ui/ctrl/CtrlMenuCourseSelectCupSub.hpp>

class CtrlMenuCourseSelectCup : public LayoutUIControl {
public:
    virtual ~CtrlMenuCourseSelectCup();

    virtual void initSelf();

    virtual const TypeInfo* getTypeInfo() const;
    virtual const char* getTypeName() const;

    CtrlMenuCourseSelectCupSub cups[8];
};
size_assert(CtrlMenuCourseSelectCup, 0xDD4);
