#include <kamek.h>
#include <game/ui/ctrl/CtrlMenuMovieLayout.h>
#include <game/ui/InputHandler.h>
#include <game/ui/LayoutUIControl.h>
#include <game/ui/PushButton.h>

class CtrlMenuBattleCupSelectCup : public LayoutUIControl {
public:
    virtual ~CtrlMenuBattleCupSelectCup();
    virtual void initSelf();

    virtual const TypeInfo* getTypeInfo() const;
    virtual const char* getTypeName() const;

    PushButton cupButtons[2];
    InputHandler0<CtrlMenuBattleCupSelectCup, void> onClickHandler;
    InputHandler0<CtrlMenuBattleCupSelectCup, void> onSelectHandler;
    u32 currentSelected;
    CtrlMenuMovieLayout movieLayout;
};
size_assert(CtrlMenuBattleCupSelectCup, 0x7D4);
