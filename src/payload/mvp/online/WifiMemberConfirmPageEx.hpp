#include <common/Common.hpp>
#include <game/ui/page/MessagePopupPage.hpp>
#include <game/ui/page/WifiMemberConfirmPage.hpp>

class WifiMemberConfirmPageEx : public WifiMemberConfirmPage {
public:
    WifiMemberConfirmPageEx() : ruleButton(), changeComboButton(),
                                onRuleBtnPress(this, &handleRuleButtonPress), rulePopup(nullptr) {}
    virtual ~WifiMemberConfirmPageEx() {}

    virtual void onInit();
    virtual void onActivate();
    virtual void afterCalc();

    void handleRuleButtonPress(PushButton* button, u32 playerFlags);

    PushButton ruleButton;
    PushButton changeComboButton;
    InputHandler2<WifiMemberConfirmPageEx, void, PushButton*, u32> onRuleBtnPress;
    MessagePopupPage* rulePopup;
};
