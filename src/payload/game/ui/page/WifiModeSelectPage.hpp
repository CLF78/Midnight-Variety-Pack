#include <common/Common.hpp>
#include <game/ui/InputHandler.hpp>
#include <game/ui/MultiControlInputManager.hpp>
#include <game/ui/Page.hpp>
#include <game/ui/PushButton.hpp>
#include <game/ui/ctrl/CtrlMenuBackButton.hpp>
#include <game/ui/ctrl/CtrlMenuInstructionText.hpp>
#include <game/ui/ctrl/CtrlMenuPageTitleText.hpp>

class WifiModeSelectPage : public Page {
public:
    WifiModeSelectPage();
    virtual ~WifiModeSelectPage();

    virtual int getReplacement();

    virtual void onInit();
    virtual void onActivate();

    virtual TypeInfo* getTypeInfo() const;

    InputHandler1<WifiModeSelectPage, void, PushButton*> onButtonPress;
    InputHandler1<WifiModeSelectPage, void, PushButton*> onBackButtonPress;
    InputHandler1<WifiModeSelectPage, void, PushButton*> onButtonSelect;
    InputHandler0<WifiModeSelectPage, void> onBack;

    CtrlMenuPageTitleText titleText;
    PushButton raceButton;
    PushButton battleButton;
    CtrlMenuBackButton backButton;
    CtrlMenuInstructionText instructionText;

    MultiControlInputManager inputManager;
    int replacementPage;
};
size_assert(WifiModeSelectPage, 0xCB0);
