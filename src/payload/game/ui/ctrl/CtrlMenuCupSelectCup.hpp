#include <common/Common.hpp>
#include <game/ui/ctrl/CtrlMenuCupSelectCourse.hpp>
#include <game/ui/ctrl/CtrlMenuMovieLayout.hpp>
#include <game/ui/InputHandler.hpp>
#include <game/ui/LayoutUIControl.hpp>
#include <game/ui/PushButton.hpp>

class CtrlMenuCupSelectCup : public LayoutUIControl {
public:
    virtual ~CtrlMenuCupSelectCup();
    virtual void initSelf();

    virtual const TypeInfo* getTypeInfo() const;
    virtual const char* getTypeName() const;

    void onCupSelect(PushButton* btn, u32 unk);

    PushButton cupButtons[8];
    InputHandler2<PushButton, void, PushButton*, u32> onClickHandler;
    InputHandler2<PushButton, void, PushButton*, u32> onSelectHandler;
    u32 currentSelected;
    CtrlMenuMovieLayout movieLayout;
};
size_assert(CtrlMenuCupSelectCup, 0x15CC);
