#pragma once
#include <common/Common.hpp>
#include <game/ui/ctrl/CtrlRaceBase.hpp>

class CtrlRaceMessageDisplay : public CtrlRaceBase {
public:

    enum State {
        INVISIBLE,
        VISIBLE,
        FADE_OUT,
    };

    CtrlRaceMessageDisplay() : textBox(nullptr), timer(0), state(INVISIBLE) {}
    virtual ~CtrlRaceMessageDisplay() {}

    virtual void calcSelf();

    virtual nw4r::lyt::Pane* getPane() { return textBox; }
    virtual bool shouldHide();
    virtual bool shouldActivate();

    RUNTIME_TYPE_INFO;
    RUNTIME_TYPENAME_INFO(CtrlRaceMessageDisplay);

    void load(u32 localPlayerCount, u32 playerIdx, u32 queuePos);
    void showMessage();

    nw4r::lyt::Pane* textBox;
    u16 timer;
    State state;
};
