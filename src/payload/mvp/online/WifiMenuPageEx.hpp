#pragma once
#include <common/Common.hpp>
#include <game/ui/page/WifiMenuPage.hpp>

class WifiMenuPageEx : public WifiMenuPage {
public:
    WifiMenuPageEx() {}

    virtual void onInit();
    virtual void beforeCalc();
};
