#pragma once
#include <common/Common.hpp>
#include <game/ui/UIControlTimer.hpp>
#include <game/ui/page/MenuPage.hpp>
#include <game/ui/ctrl/CtrlMenuCupSelectCup.hpp>
#include <game/ui/ctrl/CtrlMenuCupSelectCourse.hpp>
#include <mvp/online/YesNoPopupPageEx.hpp>

class RaceCupSelectPage : public MenuPage {
public:
    RaceCupSelectPage();
    virtual ~RaceCupSelectPage();

    virtual void onInit();
    virtual void onActivate();
    virtual void onDeactivate();
    virtual void beforeInAnim();
    virtual void beforeOutAnim();
    virtual void afterCalc();
    virtual void onRefocus();

    RUNTIME_TYPE_INFO_NODECL;

    virtual void onSelectChange(PushButton* button, u32 hudSlotId);
    virtual int getActivePlayers();
    virtual int getActiveControllers();
    virtual MenuInputManager* getInputManager();

    virtual PushButton* loadButton(int buttonIdx);
    virtual UIControl* loadLayout(u32 layoutIdx);
    virtual void setupButton(PushButton* button);

    CtrlMenuCupSelectCup cupHolder;
    CtrlMenuCupSelectCourse courseHolder;
    u32 selectedButtonId;

    YesNoPopupPageEx* voteOrRandomPage;
    UIControlTimer* timer;
};
size_assert(RaceCupSelectPage, 0x23E0);
