#include <common/Common.hpp>
#include <game/ui/LayoutUIControl.hpp>

class CtrlMenuCupSelectCourse : public LayoutUIControl {
public:
    virtual ~CtrlMenuCupSelectCourse();

    virtual void initSelf();
    virtual void calcSelf();

    virtual const TypeInfo* getTypeInfo() const;
    virtual const char* getTypeName() const;

    void setCourseNames(u32 cupButtonId); // replaced function

    LayoutUIControl courseNames[4];
};
size_assert(CtrlMenuCupSelectCourse, 0x744);
