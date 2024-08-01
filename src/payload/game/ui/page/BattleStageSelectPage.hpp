#pragma once
#include "MenuPage.hpp"
#include <game/ui/UIControlTimer.hpp>
#include <game/ui/ctrl/CtrlMenuBattleStageSelectCup.hpp>
#include <game/ui/ctrl/CtrlMenuBattleStageSelectStage.hpp>

class BattleStageSelectPage : public MenuPage {
public:
    BattleStageSelectPage();
    virtual ~BattleStageSelectPage();

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

    CtrlMenuBattleStageSelectCup cupHolder;
    CtrlMenuBattleStageSelectStage courseHolder;
    bool stageSelected;
};
size_assert(BattleStageSelectPage, 0x1A84);
