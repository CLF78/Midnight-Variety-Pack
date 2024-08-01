#pragma once
#include "LayoutUIControl.hpp"
#include <game/ui/UIControlTimer.hpp>

class CountdownTimerControl : public LayoutUIControl {
public:
    virtual ~CountdownTimerControl();

    virtual void initSelf();
    virtual void calcSelf();

    RUNTIME_TYPE_INFO_NODECL;
    RUNTIME_TYPENAME_INFO_NODECL;

    UIControlTimer* timer;
    bool _178;
};
size_assert(CountdownTimerControl, 0x17C);
