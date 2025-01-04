#include <game/ui/ctrl/LayoutUIControl.hpp>

class MatchingMessageWindow : public LayoutUIControl {
public:
    virtual ~MatchingMessageWindow();

    virtual void initSelf();
    virtual void calcSelf();

    RUNTIME_TYPE_INFO_NODECL;
    RUNTIME_TYPENAME_INFO_NODECL;
};
size_assert(MatchingMessageWindow, sizeof(LayoutUIControl));
