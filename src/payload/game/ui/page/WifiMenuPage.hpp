#include <common/Common.hpp>
#include <game/ui/InputHandler.hpp>
#include <game/ui/LayoutUIControl.hpp>
#include <game/ui/MultiControlInputManager.hpp>
#include <game/ui/Page.hpp>
#include <game/ui/PushButton.hpp>
#include <game/ui/ctrl/CtrlMenuBackButton.hpp>
#include <game/ui/ctrl/CtrlMenuInstructionText.hpp>
#include <game/ui/ctrl/FriendStatusButton.hpp>

class WifiMenuPage : public Page {
public:
    WifiMenuPage();
    virtual ~WifiMenuPage();

    virtual int getReplacement();

    virtual void onInit();
    virtual void onActivate();

    virtual TypeInfo* getTypeInfo() const;

    InputHandler1<WifiMenuPage, void, PushButton*> onWorldwideButtonPressed;
    InputHandler1<WifiMenuPage, void, PushButton*> onRegionalButtonPressed;
    InputHandler1<WifiMenuPage, void, PushButton*> onFriendButtonPressed;
    InputHandler1<WifiMenuPage, void, PushButton*> onBackButtonPressed;
    InputHandler1<WifiMenuPage, void, PushButton*> onButtonSelect;
    InputHandler0<WifiMenuPage, void> onBack;

    LayoutUIControl titleText;
    PushButton worldwideButton;
    PushButton regionalButton;
    FriendStatusButton friendButton;

    CtrlMenuBackButton backButton;
    CtrlMenuInstructionText instructionText;

    MultiControlInputManager inputManager;
    int replacementPage;
};
size_assert(WifiMenuPage, 0xF34);

class WifiMenuPageEx : public WifiMenuPage {
public:
    WifiMenuPageEx() : WifiMenuPage() {}

    virtual void beforeCalc();
};
