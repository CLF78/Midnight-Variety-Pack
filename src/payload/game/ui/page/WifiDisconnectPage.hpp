#pragma once
#include <game/net/WifiDisconnectInfo.hpp>
#include <game/ui/ctrl/LayoutUIControl.hpp>
#include "Page.hpp"
#include <game/ui/input/PageInputManager.hpp>
#include <game/ui/ctrl/PushButton.hpp>

class WifiDisconnectPage : public Page {
public:
    virtual ~WifiDisconnectPage();

    virtual int vf_14();

    virtual void onInit();
    virtual void onActivate();
    virtual void beforeInAnim();
    virtual void afterCalc();

    RUNTIME_TYPE_INFO_NODECL;

    PageInputManager inputManager;
    WifiDisconnectInfo disconnectInfo;
    LayoutUIControl messageBox;
    LayoutUIControl okButton;
    InputHandler2<WifiDisconnectPage, void, PushButton*, u32> onButtonSelectHandler;
};
size_assert(WifiDisconnectPage, 0x48C);
