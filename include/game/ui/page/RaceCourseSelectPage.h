#include <kamek.h>
#include <game/ui/page/MenuPage.h>
#include <game/ui/MultiControlInputManager.h>
#include <game/ui/UIControlTimer.h>
#include <game/ui/ctrl/CtrlMenuCourseSelectCup.h>
#include <game/ui/ctrl/CtrlMenuCourseSelectCourse.h>

class RaceCourseSelectPage : public MenuPage {
public:
    MultiControlInputManager multiControlInputManager;
    u32 curChildCount;

    InputHandler0<RaceCourseSelectPage, void> onBtnClick;
    InputHandler0<RaceCourseSelectPage, void> onBtnSelect;
    InputHandler0<RaceCourseSelectPage, void> onBtnDeselect;
    InputHandler0<RaceCourseSelectPage, void> onBackPress;
    InputHandler0<RaceCourseSelectPage, void> onStartPress;

    u32 activePlayers;
    u32 activeControllers;

    UIControlTimer* timer;

    CtrlMenuCourseSelectCup cupHolder;
    CtrlMenuCourseSelectCourse courseHolder;
    bool _215C;
};
size_assert(RaceCourseSelectPage, 0x2160);
