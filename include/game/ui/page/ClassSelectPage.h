#include <kamek.h>
#include <game/ui/page/MenuPage.h>
#include <game/ui/MultiControlInputManager.h>

class ClassSelectPage : public MenuPage {
public:
    MultiControlInputManager inputManager;
    u32 curBtnIndex;

    InputHandler1<ClassSelectPage, void, PushButton> onButtonPress;
    InputHandler1<ClassSelectPage, void, PushButton> onButtonSelect;
    InputHandler1<ClassSelectPage, void, PushButton> onButtonDeselect;
    InputHandler0<ClassSelectPage, void> onBackPress;
    InputHandler0<ClassSelectPage, void> onStartPressHandler;

    u32 activePlayers;
    u32 activeControllers;
    bool switchModeOff;
    LayoutUIControl switchButton;

    static u32 engineClasses[6];
};
size_assert(ClassSelectPage, 0x83C);
