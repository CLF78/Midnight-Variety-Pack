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

    virtual const TypeInfo* getTypeInfo() const;
    virtual const char* getTypeName() const;

    PushButton cupButtons[2];
    InputHandler0<PushButton, void> onClickHandler;
    InputHandler0<PushButton, void> onSelectHandler;
    u32 currentSelected;
    CtrlMenuMovieLayout movieLayout; // unused
};
size_assert(CtrlMenuBattleCupSelectCup, 0x7D4);
