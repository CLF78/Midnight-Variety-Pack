#include <common/Common.hpp>
#include <game/ui/ctrl/CtrlMenuMovieLayout.hpp>
#include <game/ui/InputHandler.hpp>
#include <game/ui/LayoutUIControl.hpp>
#include <game/ui/PushButton.hpp>

class BattleCupSelectPage;

class CtrlMenuBattleCupSelectCup : public LayoutUIControl {
public:
    virtual ~CtrlMenuBattleCupSelectCup();
    virtual void initSelf();

    RUNTIME_TYPE_INFO_NODECL;
    RUNTIME_TYPENAME_INFO_NODECL;

    void load(u32 playerFlags, bool unk);

    void onCupClick(PushButton* btn, u32 unk);
    void onCupSelect(PushButton* btn, u32 unk);

    PushButton cupButtons[2];
    InputHandler2<PushButton, void, PushButton*, u32> onClickHandler;
    InputHandler2<PushButton, void, PushButton*, u32> onSelectHandler;
    u32 currentSelected;
    CtrlMenuMovieLayout movieLayout; // unused
};
size_assert(CtrlMenuBattleCupSelectCup, 0x7D4);
