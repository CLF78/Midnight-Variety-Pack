#include <common/Common.hpp>
#include <game/ui/ctrl/PushButton.hpp>

class CtrlMenuBackButton : public PushButton {
public:
    virtual ~CtrlMenuBackButton();

    virtual void initSelf();
    virtual void calcSelf();
    virtual void solveAnim(Element* element, float frame);

    RUNTIME_TYPE_INFO_NODECL;
    RUNTIME_TYPENAME_INFO_NODECL;

    int bmgId;
    u32 fadeoutState;
    float fadeoutOpacity;
    nw4r::lyt::Pane* pane;
};
size_assert(CtrlMenuBackButton, 0x264);
