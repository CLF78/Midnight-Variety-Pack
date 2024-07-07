#include <common/Common.hpp>
#include <game/ui/LayoutUIControl.hpp>

class CtrlRaceBase : public LayoutUIControl {
public:
    enum State {
        EXITING_SCREEN,
        HIDDEN,
        ENTERING_SCREEN,
        DISPLAYED,
    };

    CtrlRaceBase() {}
    virtual ~CtrlRaceBase();

    virtual void initSelf();

    RUNTIME_TYPE_INFO_NODECL;
    RUNTIME_TYPENAME_INFO_NODECL;

    virtual void process();
    virtual void updatePausePosition(float delta);
    virtual nw4r::lyt::Pane* getPane();

    virtual bool shouldHide();
    virtual bool shouldActivate();

    u32 screenState;
    VEC3 posBackup;
    float delta;
    u32 _184;

    nw4r::lyt::Pane* pane;
    u8 localPlayerIdx;
    // 3 bytes padding

    float* currAnimFrames;
};
size_assert(CtrlRaceBase, 0x198);
