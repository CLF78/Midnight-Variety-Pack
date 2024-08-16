#pragma once
#include "CtrlMenuMovieButton.hpp"
#include "LayoutUIControl.hpp"

class CtrlMenuCourseSelectCourse : public LayoutUIControl {
public:
    virtual ~CtrlMenuCourseSelectCourse();

    virtual void initSelf();

    RUNTIME_TYPE_INFO_NODECL;
    RUNTIME_TYPENAME_INFO_NODECL;

    void onButtonClick(PushButton* button, u32 hudSlotId);
    void onButtonSelect(PushButton* button, u32 hudSlotId);

    CtrlMenuMovieButton courseButtons[4];
    LayoutUIControl _B24;
    InputHandler2<CtrlMenuCourseSelectCourse, void, PushButton*, u32> onButtonClickHandler;
    InputHandler2<CtrlMenuCourseSelectCourse, void, PushButton*, u32> onButtonSelectHandler;
};
size_assert(CtrlMenuCourseSelectCourse, 0xCC0);
