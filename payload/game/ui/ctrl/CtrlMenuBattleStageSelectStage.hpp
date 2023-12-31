#include <common/Common.hpp>
#include <game/ui/LayoutUIControl.hpp>
#include <game/ui/ctrl/CtrlMenuMovieButton.hpp>

class CtrlMenuBattleStageSelectStage : public LayoutUIControl {
public:
    virtual ~CtrlMenuBattleStageSelectStage();

    virtual void initSelf();

    virtual const TypeInfo* getTypeInfo() const;
    virtual const char* getTypeName() const;

    CtrlMenuMovieButton courseButtons[4];
    CtrlMenuMovieButton unusedButton;
    LayoutUIControl _D90;
    InputHandler0<PushButton, void> onClickHandler;
    InputHandler0<PushButton, void> onSelectHandler;

    static const char* buttonAnims[6];
};
size_assert(CtrlMenuBattleStageSelectStage, 0xF2C);

