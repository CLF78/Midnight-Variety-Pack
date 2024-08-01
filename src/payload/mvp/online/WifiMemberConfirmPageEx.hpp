#pragma once
#include <game/ui/page/MessagePopupPage.hpp>
#include <game/ui/page/WifiMemberConfirmPage.hpp>

class WifiMemberConfirmPageEx : public WifiMemberConfirmPage {
public:
    WifiMemberConfirmPageEx() : ruleButton(), changeComboButton(),
                                onRuleButtonClickHandler(this, &onRuleButtonClick),
                                rulePopup(nullptr) {}
    virtual ~WifiMemberConfirmPageEx() {}

    virtual void onInit();
    virtual void onActivate();
    virtual void afterCalc();

    void onRuleButtonClick(PushButton* button, u32 playerFlags);

    PushButton ruleButton;
    PushButton changeComboButton;
    InputHandler2<WifiMemberConfirmPageEx, void, PushButton*, u32> onRuleButtonClickHandler;
    MessagePopupPage* rulePopup;
};
