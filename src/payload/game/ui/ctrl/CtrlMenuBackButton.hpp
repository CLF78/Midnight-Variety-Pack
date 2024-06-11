#include <common/Common.hpp>
#include <game/ui/PushButton.hpp>

class CtrlMenuBackButton : public PushButton {
public:
    virtual ~CtrlMenuBackButton();

    virtual void initSelf();
    virtual void calcSelf();
    virtual void solveAnim(Element* element, float frame);

    virtual const TypeInfo* getTypeInfo() const;
    virtual const char* getTypeName() const;

    int bmgId;
    u32 fadeoutState;
    float fadeoutOpacity;
    nw4r::lyt::Pane* pane;
};
size_assert(CtrlMenuBackButton, 0x264);
