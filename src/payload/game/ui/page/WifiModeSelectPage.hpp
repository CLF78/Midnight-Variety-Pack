#pragma once
#include "Page.hpp"
#include <game/ui/InputHandler.hpp>
#include <game/ui/ctrl/CtrlMenuBackButton.hpp>
#include <game/ui/ctrl/CtrlMenuInstructionText.hpp>
#include <game/ui/ctrl/CtrlMenuPageTitleText.hpp>
#include <game/ui/ctrl/PushButton.hpp>
#include <game/ui/input/MultiControlInputManager.hpp>

class WifiModeSelectPage : public Page {
public:
    WifiModeSelectPage();
    virtual ~WifiModeSelectPage();

    virtual int getReplacement();

    virtual void onInit();
    virtual void onActivate();

    RUNTIME_TYPE_INFO_NODECL;

    InputHandler2<WifiModeSelectPage, void, PushButton*, u32> onButtonClickHandler;
    InputHandler2<WifiModeSelectPage, void, PushButton*, u32> onBackButtonClickHandler;
    InputHandler2<WifiModeSelectPage, void, PushButton*, u32> onButtonSelectHandler;
    InputHandler1<WifiModeSelectPage, void, u32> onBackHandler;

    CtrlMenuPageTitleText titleText;
    PushButton raceButton;
    PushButton battleButton;
    CtrlMenuBackButton backButton;
    CtrlMenuInstructionText instructionText;

    MultiControlInputManager inputManager;
    int replacementPage;
};
size_assert(WifiModeSelectPage, 0xCB0);
