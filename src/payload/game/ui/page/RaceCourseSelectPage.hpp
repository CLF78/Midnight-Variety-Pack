#include <common/Common.hpp>
#include <game/ui/page/MenuPage.hpp>
#include <game/ui/SectionManager.hpp>
#include <game/ui/UIControlTimer.hpp>
#include <game/ui/ctrl/CtrlMenuCourseSelectCup.hpp>
#include <game/ui/ctrl/CtrlMenuCourseSelectCourse.hpp>

class RaceCourseSelectPage : public MenuPage {
public:
    UIControlTimer* timer;

    CtrlMenuCourseSelectCup cupHolder;
    CtrlMenuCourseSelectCourse courseHolder;
    bool courseSelected;

    static RaceCourseSelectPage* getPage() {
        return (RaceCourseSelectPage*)SectionManager::instance->curSection->pages[Page::COURSE_SELECT];
    }
};
size_assert(RaceCourseSelectPage, 0x2160);
