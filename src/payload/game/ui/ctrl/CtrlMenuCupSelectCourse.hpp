#pragma once
#include "LayoutUIControl.hpp"

class CtrlMenuCupSelectCourse : public LayoutUIControl {
public:
    virtual ~CtrlMenuCupSelectCourse();

    virtual void initSelf();
    virtual void calcSelf();

    RUNTIME_TYPE_INFO_NODECL;
    RUNTIME_TYPENAME_INFO_NODECL;

    void load();
    void setCourseNames(u32 cupButtonId);

    LayoutUIControl courseNames[4];
};
size_assert(CtrlMenuCupSelectCourse, 0x744);
