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

    InputHandler<RaceCourseSelectPage> onBtnClick;
    InputHandler<RaceCourseSelectPage> onBtnSelect;
    InputHandler<RaceCourseSelectPage> onBtnDeselect;
    InputHandler<RaceCourseSelectPage> onBackPress;
    InputHandler<RaceCourseSelectPage> onStartPress;

    u32 activePlayers;
    u32 activeControllers;

    UIControlTimer* timer;

    CtrlMenuCourseSelectCup cupHolder;
    CtrlMenuCourseSelectCourse courseHolder;
    bool _215C;
};
size_assert(RaceCourseSelectPage, 0x2160);
