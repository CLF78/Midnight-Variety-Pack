#include <kamek.h>
#include <game/ui/LayoutUIControl.h>
#include <nw4r/lyt/pane.h>
#include <rvl/mtx/mtx.h>

class CtrlMenuCourseSelectCupSub : public LayoutUIControl {
public:
    virtual ~CtrlMenuCourseSelectCupSub();

    virtual void initSelf();
    virtual void calcSelf();

    virtual const TypeInfo* getTypeInfo() const;
    virtual const char* getTypeName() const;

    float alpha;
    nw4r::lyt::Pane* rootPane;
    VEC3 somePos;
    bool selected;
    u8 fadeDirection; // 0 = entering, 1 = exiting
};
size_assert(CtrlMenuCourseSelectCupSub, 0x18C);
