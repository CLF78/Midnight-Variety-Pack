#pragma once
#include "CtrlMenuCourseSelectCupSub.hpp"
#include "LayoutUIControl.hpp"

class CtrlMenuCourseSelectCup : public LayoutUIControl {
public:
    virtual ~CtrlMenuCourseSelectCup();

    virtual void initSelf();

    RUNTIME_TYPE_INFO_NODECL;
    RUNTIME_TYPENAME_INFO_NODECL;

    CtrlMenuCourseSelectCupSub cups[8];
};
size_assert(CtrlMenuCourseSelectCup, 0xDD4);
