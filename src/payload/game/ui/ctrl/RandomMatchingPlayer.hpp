#include <game/system/Mii.hpp>
#include <game/ui/InputHandler.hpp>
#include <game/ui/ctrl/LayoutUIControl.hpp>
#include <nw4r/lyt/pane.hpp>

class RandomMatchingPlayer : public LayoutUIControl {
public:
    virtual ~RandomMatchingPlayer();

    virtual void initSelf();
    virtual void calcSelf();

    RUNTIME_TYPE_INFO_NODECL;
    RUNTIME_TYPENAME_INFO_NODECL;

    InputHandler0<RandomMatchingPlayer, void> onMiiShowing;
    nw4r::lyt::Pane* allNull;
    u32 localPlayerCount;
    u32 playerIdx;
    UNK(8);

    float angle;
    UNK(8);

    MiiGroup* miiGroup;
    u32 miiIdx;
    UNK(8);
};
size_assert(RandomMatchingPlayer, 0x1B8);
