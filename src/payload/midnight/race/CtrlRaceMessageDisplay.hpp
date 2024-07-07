#include <common/Common.hpp>
#include <game/ui/LayoutUIControl.hpp>

class CtrlRaceMessageDisplay : public LayoutUIControl {
public:

    enum State {
        INVISIBLE,
        VISIBLE,
        FADE_OUT,
    };

    CtrlRaceMessageDisplay() : textBox(nullptr), timer(0), state(INVISIBLE) {}
    virtual ~CtrlRaceMessageDisplay() {}

    virtual void calcSelf();

    RUNTIME_TYPE_INFO;
    RUNTIME_TYPENAME_INFO(CtrlRaceMessageDisplay);

    void load(u32 localPlayerCount, u32 playerIdx, u32 queuePos);
    void showMessage();

    nw4r::lyt::Pane* textBox;
    u16 timer;
    State state;
};
