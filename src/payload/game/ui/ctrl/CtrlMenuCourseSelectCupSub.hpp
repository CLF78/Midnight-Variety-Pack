#include <common/Common.hpp>
#include <game/ui/LayoutUIControl.hpp>
#include <nw4r/lyt/pane.hpp>
#include <revolution/mtx.h>

class CtrlMenuCourseSelectCupSub : public LayoutUIControl {
public:
    virtual ~CtrlMenuCourseSelectCupSub();

    virtual void initSelf();
    virtual void calcSelf();

    RUNTIME_TYPE_INFO_NODECL;
    RUNTIME_TYPENAME_INFO_NODECL;

    virtual void vf_3C(VEC3* vector);

    float alpha;
    nw4r::lyt::Pane* rootPane;
    VEC3 somePos;
    bool selected;
    u8 fadeDirection; // 0 = entering, 1 = exiting
};
size_assert(CtrlMenuCourseSelectCupSub, 0x18C);
