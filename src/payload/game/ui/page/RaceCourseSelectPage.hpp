#include <common/Common.hpp>
#include <game/ui/page/MenuPage.hpp>
#include <game/ui/SectionManager.hpp>
#include <game/ui/UIControlTimer.hpp>
#include <game/ui/ctrl/CtrlMenuCourseSelectCup.hpp>
#include <game/ui/ctrl/CtrlMenuCourseSelectCourse.hpp>

class RaceCourseSelectPage : public MenuPage {
public:
    virtual ~RaceCourseSelectPage();

    virtual void onInit();
    virtual void onActivate();
    virtual void onDeactivate();

    virtual void beforeInAnim();
    virtual void beforeOutAnim();
    virtual void afterCalc();

    virtual TypeInfo* getTypeInfo() const;

    virtual void onSelectChange(PushButton* button, int playerId);
    virtual int getActivePlayers();
    virtual int getActiveControllers();
    virtual MenuInputManager* getInputManager();

    virtual PushButton* loadButton(int buttonIdx);
    virtual UIControl* loadLayout(u32 layoutIdx);
    virtual void setupButton(PushButton* button);

    void setCourse(CtrlMenuCourseSelectCourse* courseHolder, PushButton* button);

    UIControlTimer* timer;

    CtrlMenuCourseSelectCup cupHolder;
    CtrlMenuCourseSelectCourse courseHolder;
    bool courseSelected;

    static RaceCourseSelectPage* getPage() {
        return (RaceCourseSelectPage*)SectionManager::instance->curSection->pages[Page::COURSE_SELECT];
    }
};
size_assert(RaceCourseSelectPage, 0x2160);
