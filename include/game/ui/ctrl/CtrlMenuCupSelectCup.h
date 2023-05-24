#include <kamek.h>
#include <game/ui/ctrl/CtrlMenuCupSelectCourse.h>
#include <game/ui/ctrl/CtrlMenuMovieLayout.h>
#include <game/ui/InputHandler.h>
#include <game/ui/LayoutUIControl.h>
#include <game/ui/PushButton.h>

class CtrlMenuCupSelectCup : public LayoutUIControl {
public:
    virtual ~CtrlMenuCupSelectCup();
    virtual void initSelf();

    virtual const TypeInfo* getTypeInfo() const;
    virtual const char* getTypeName() const;

    PushButton cupButtons[8];
    InputHandler0<CtrlMenuCupSelectCup, void> onClickHandler;
    InputHandler0<CtrlMenuCupSelectCup, void> onSelectHandler;
    u32 currentSelected;
    CtrlMenuMovieLayout movieLayout;
    CtrlMenuCupSelectCourse courseHolder;
};
size_assert(CtrlMenuCupSelectCup, 0x1D10);
