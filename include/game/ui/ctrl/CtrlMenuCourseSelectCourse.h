#include <kamek.h>
#include <game/ui/LayoutUIControl.h>
#include <game/ui/ctrl/CtrlMenuMovieButton.h>

class CtrlMenuCourseSelectCourse : public LayoutUIControl {
public:
    virtual ~CtrlMenuCourseSelectCourse();

    virtual void initSelf();

    virtual const TypeInfo* getTypeInfo() const;
    virtual const char* getTypeName() const;

    CtrlMenuMovieButton courseButtons[4];
    LayoutUIControl _B24;
    InputHandler0<CtrlMenuCourseSelectCourse, void> onClickHandler;
    InputHandler0<CtrlMenuCourseSelectCourse, void> onSelectHandler;
};
size_assert(CtrlMenuCourseSelectCourse, 0xCC0);
