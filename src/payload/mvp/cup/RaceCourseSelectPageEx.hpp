#pragma once
#include <common/Common.hpp>
#include <game/ui/page/RaceCourseSelectPage.hpp>
#include <mvp/online/YesNoPopupPageEx.hpp>

class RaceCourseSelectPageEx : public RaceCourseSelectPage {
public:
    RaceCourseSelectPageEx() : onRepickPromptPressHandler(this, &onRepickPromptPress),
                               repickPrompt(nullptr) {}
    virtual ~RaceCourseSelectPageEx() {}

    virtual void onActivate();
    virtual void afterCalc();

    void onRepickPromptPress(s32 choice, PushButton* button); // custom function
    void setCourse(CtrlMenuCourseSelectCourse* courseHolder, PushButton* button, int unk);

    static RaceCourseSelectPageEx* getPage() {
        return (RaceCourseSelectPageEx*)SectionManager::instance->curSection->getPage(Page::COURSE_SELECT);
    }

    InputHandler2<RaceCourseSelectPageEx, void, s32, PushButton*> onRepickPromptPressHandler;
    YesNoPopupPageEx* repickPrompt;
};
