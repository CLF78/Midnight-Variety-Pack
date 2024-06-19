#include <common/Common.hpp>
#include <game/ui/LayoutUIControl.hpp>
#include <game/ui/ctrl/CtrlMenuMovieButton.hpp>

class CtrlMenuCourseSelectCourse : public LayoutUIControl {
public:
    virtual ~CtrlMenuCourseSelectCourse();

    virtual void initSelf();

    virtual const TypeInfo* getTypeInfo() const;
    virtual const char* getTypeName() const;

    void onCourseClick(PushButton* btn, u32 unk);
    void onCourseSelect(PushButton* btn, u32 unk);

    CtrlMenuMovieButton courseButtons[4];
    LayoutUIControl _B24;
    InputHandler0<CtrlMenuCourseSelectCourse, void> onClickHandler;
    InputHandler0<CtrlMenuCourseSelectCourse, void> onSelectHandler;
};
size_assert(CtrlMenuCourseSelectCourse, 0xCC0);
