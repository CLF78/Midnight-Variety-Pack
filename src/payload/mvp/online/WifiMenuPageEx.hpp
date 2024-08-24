#pragma once
#include <game/ui/page/WifiMenuPage.hpp>

class WifiMenuPageEx : public WifiMenuPage {
public:
    WifiMenuPageEx() {}

    virtual void onInit();
    virtual void beforeCalc();
};
