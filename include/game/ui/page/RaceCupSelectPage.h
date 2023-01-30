#include <kamek.h>
#include <game/ui/ctrl/CtrlMenuCupSelectCup.h>
#include <game/ui/ctrl/SheetSelectControl.h>
#include <game/ui/page/MenuPage.h>
#include <game/ui/MultiControlInputManager.h>

// Expansion structure for the custom cup system
#ifdef CUSTOM_CUP_SYSTEM
struct RaceCupSelectPageEx {
    SheetSelectControl arrows;
};
#else
struct RaceCupSelectPageEx;
#endif

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

    RaceCupSelectPageEx extension;

};
size_assert(RaceCupSelectPage, 0x23E0 + sizeof(RaceCupSelectPageEx));
