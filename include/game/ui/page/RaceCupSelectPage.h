#include <kamek.h>
#include <game/ui/ctrl/CtrlMenuCupSelectCup.h>
#include <game/ui/page/MenuPage.h>
#include <game/ui/MultiControlInputManager.h>

class UIControlTimer;

class RaceCupSelectPage : public MenuPage {
public:
    MultiControlInputManager multiControlInputManager;
    u32 curChildCount;

    InputHandler<RaceCupSelectPage> onBackClick;
    InputHandler<RaceCupSelectPage> onBtnSelect;
    InputHandler<RaceCupSelectPage> onBtnDeselect;
    InputHandler<RaceCupSelectPage> onBackPress;
    InputHandler<RaceCupSelectPage> onStart;

    u32 activePlayers;
    u32 activeControllers;

    CtrlMenuCupSelectCup cupHolder;
    u32 selectedButton;

    Page* voteOrRandomPage;
    UIControlTimer* timer;
};
size_assert(RaceCupSelectPage, 0x23E0);
