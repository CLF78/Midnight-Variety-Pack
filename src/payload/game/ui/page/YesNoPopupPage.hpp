#pragma once
#include <game/ui/SectionManager.hpp>
#include <game/ui/ctrl/BlackBackControl.hpp>
#include "YesNoPage.hpp"

class YesNoPopupPage : public YesNoPage {
public:
    YesNoPopupPage();
    virtual ~YesNoPopupPage();

    virtual void onInit();
    virtual void beforeInAnim();
    virtual void afterOutAnim();

    RUNTIME_TYPE_INFO_NODECL;

    virtual void reset();

    BlackBackControl blackBackControl;
    LayoutUIControl messagePopup;
};
size_assert(YesNoPopupPage, 0xBA0);
