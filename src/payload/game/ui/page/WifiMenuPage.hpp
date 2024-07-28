#pragma once
#include <common/Common.hpp>
#include <game/ui/InputHandler.hpp>
#include <game/ui/ctrl/LayoutUIControl.hpp>
#include <game/ui/input/MultiControlInputManager.hpp>
#include <game/ui/page/Page.hpp>
#include <game/ui/ctrl/PushButton.hpp>
#include <game/ui/ctrl/CtrlMenuBackButton.hpp>
#include <game/ui/ctrl/CtrlMenuInstructionText.hpp>
#include <game/ui/ctrl/CtrlMenuPageTitleText.hpp>
#include <game/ui/ctrl/FriendStatusButton.hpp>

class WifiMenuPage : public Page {
public:
    enum ButtonId {
        BTN_WORLDWIDE = 1,
        BTN_REGIONAL = 2,
        BTN_FRIENDS = 3,
        BTN_BACK = 4,
    };

    WifiMenuPage();
    virtual ~WifiMenuPage();

    virtual int getReplacement();

    virtual void onInit();
    virtual void onActivate();

    RUNTIME_TYPE_INFO_NODECL;

    InputHandler2<WifiMenuPage, void, PushButton*, u32> onWorldwideButtonClickHandler;
    InputHandler2<WifiMenuPage, void, PushButton*, u32> onRegionalButtonClickHandler;
    InputHandler2<WifiMenuPage, void, PushButton*, u32> onFriendButtonClickHandler;
    InputHandler2<WifiMenuPage, void, PushButton*, u32> onBackButtonClickHandler;
    InputHandler2<WifiMenuPage, void, PushButton*, u32> onButtonSelectHandler;
    InputHandler1<WifiMenuPage, void, u32> onBackHandler;

    CtrlMenuPageTitleText titleText;
    PushButton worldwideButton;
    PushButton regionalButton;
    FriendStatusButton friendButton;

    CtrlMenuBackButton backButton;
    CtrlMenuInstructionText instructionText;

    MultiControlInputManager inputManager;
    int replacementPage;
};
size_assert(WifiMenuPage, 0xF34);
