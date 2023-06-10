#include <kamek.h>
#include <game/ui/LayoutUIControl.h>
#include <game/ui/ctrl/CtrlMenuMovieButton.h>

class CtrlMenuBattleStageSelectStage : public LayoutUIControl {
public:
    virtual ~CtrlMenuBattleStageSelectStage();

    virtual void initSelf();

    virtual const TypeInfo* getTypeInfo() const;
    virtual const char* getTypeName() const;

    CtrlMenuMovieButton courseButtons[5];
    LayoutUIControl _D90;
    InputHandler0<CtrlMenuBattleStageSelectStage, void> onClickHandler;
    InputHandler0<CtrlMenuBattleStageSelectStage, void> onSelectHandler;
};
size_assert(CtrlMenuBattleStageSelectStage, 0xF2C);

