#include <common/Common.hpp>
#include <game/ui/LayoutUIControl.hpp>
#include <game/ui/ctrl/CtrlMenuMovieButton.hpp>

class CtrlMenuBattleStageSelectStage : public LayoutUIControl {
public:
    virtual ~CtrlMenuBattleStageSelectStage();

    virtual void initSelf();

    virtual const TypeInfo* getTypeInfo() const;
    virtual const char* getTypeName() const;

    void load(u32 playerFlags, bool unk);

    void onCupSelect(PushButton* btn, u32 unk);

    CtrlMenuMovieButton courseButtons[4];
    CtrlMenuMovieButton unusedButton;
    LayoutUIControl _D90;
    InputHandler2<PushButton, void, PushButton*, u32> onClickHandler;
    InputHandler2<PushButton, void, PushButton*, u32> onSelectHandler;

    static const char* buttonAnims[6];
};
size_assert(CtrlMenuBattleStageSelectStage, 0xF2C);

