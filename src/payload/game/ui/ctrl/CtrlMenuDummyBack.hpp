#include <game/ui/ctrl/LayoutUIControl.hpp>

class CtrlMenuDummyBack : public LayoutUIControl {
public:
    virtual ~CtrlMenuDummyBack();

    virtual void calcSelf();

    RUNTIME_TYPE_INFO_NODECL;
    RUNTIME_TYPENAME_INFO_NODECL;

    u32 playerFlags;
};
size_assert(CtrlMenuDummyBack, 0x178);
