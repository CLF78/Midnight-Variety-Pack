#pragma once
#include "MenuPage.hpp"
#include <game/ui/SectionManager.hpp>
#include <game/ui/UIControlTimer.hpp>
#include <game/ui/ctrl/CtrlMenuCourseSelectCourse.hpp>
#include <game/ui/ctrl/CtrlMenuCourseSelectCup.hpp>

class RaceCourseSelectPage : public MenuPage {
public:
    RaceCourseSelectPage();
    virtual ~RaceCourseSelectPage();

    virtual void onInit();
    virtual void onActivate();
    virtual void onDeactivate();

    virtual void beforeInAnim();
    virtual void beforeOutAnim();
    virtual void afterCalc();

    RUNTIME_TYPE_INFO_NODECL;

    virtual void onSelectChange(PushButton* button, u32 hudSlotId);
    virtual int getActivePlayers();
    virtual int getActiveControllers();
    virtual MenuInputManager* getInputManager();

    virtual PushButton* loadButton(int buttonIdx);
    virtual UIControl* loadLayout(u32 layoutIdx);
    virtual void setupButton(PushButton* button);

    void forceRandomVote();

    UIControlTimer* timer;

    CtrlMenuCourseSelectCup cupHolder;
    CtrlMenuCourseSelectCourse courseHolder;
    bool courseSelected;
    PAD(3);
};
size_assert(RaceCourseSelectPage, 0x2160);
