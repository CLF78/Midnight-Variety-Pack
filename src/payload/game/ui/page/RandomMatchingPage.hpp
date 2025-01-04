#pragma once
#include "Page.hpp"
#include <game/system/Mii.hpp>
#include <game/ui/InputHandler.hpp>
#include <game/ui/SectionManager.hpp>
#include <game/ui/UIControlTimer.hpp>
#include <game/ui/ctrl/CountdownTimerControl.hpp>
#include <game/ui/ctrl/CtrlMenuDummyBack.hpp>
#include <game/ui/ctrl/CtrlMenuPageTitleText.hpp>
#include <game/ui/ctrl/MatchingMessageWindow.hpp>
#include <game/ui/ctrl/RandomMatchingPlayer.hpp>
#include <game/ui/input/PageInputManager.hpp>

class RandomMatchingPage : public Page {
public:
    virtual ~RandomMatchingPage();

    virtual int getReplacement();
    virtual void onInit();
    virtual void onActivate();

    virtual void beforeInAnim();
    virtual void beforeOutAnim();
    virtual void beforeCalc();
    virtual void afterCalc();
    virtual void onRefocus();
    RUNTIME_TYPE_INFO_NODECL;

    void setupGameMode(bool isBattle);

    static RandomMatchingPage* getPage() {
        return (RandomMatchingPage*)
            SectionManager::instance->curSection->getPage(Page::GLOBAL_SEARCH_MANAGER);
    }

    UNK(4);
    PageInputManager inputManager;
    int localPlayerCount;
    CtrlMenuPageTitleText titleText;
    RandomMatchingPlayer players[12];
    MatchingMessageWindow matchingMessageWindow;
    CountdownTimerControl countdownTimerControl;
    CtrlMenuDummyBack backButton;
    UNK(0x14);

    MiiGroup miiGroup;
    UNK(4);

    UIControlTimer timer;
    InputHandler0<RandomMatchingPage, void> onBackPressHandler;
    InputHandler0<RandomMatchingPage, void> onButtonClickHandler;
    UNK(4);

    int isRegional;
    int isBattle;
    int friendToJoin;
    Page::PageId replacement;
};
size_assert(RandomMatchingPage, 0x1D04);
