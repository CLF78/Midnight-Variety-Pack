#include <common/Common.hpp>
#include <game/ui/page/RaceCourseSelectPage.hpp>

class RaceCourseSelectPageEx : public RaceCourseSelectPage {
public:
    RaceCourseSelectPageEx() : repickHandler(this, &onRepickPromptPress) {}
    virtual ~RaceCourseSelectPageEx() {}

    virtual void onActivate();

    void onRepickPromptPress(s32 choice, PushButton* button); // custom function
    void setCourse(CtrlMenuCourseSelectCourse* courseHolder, PushButton* button, int unk);

    static RaceCourseSelectPageEx* getPage() {
        return (RaceCourseSelectPageEx*)SectionManager::instance->curSection->pages[Page::COURSE_SELECT];
    }

    InputHandler2<RaceCourseSelectPageEx, void, s32, PushButton*> repickHandler;
};
