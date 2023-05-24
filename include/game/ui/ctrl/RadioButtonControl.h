#include <kamek.h>
#include <game/ui/ControlInputManager.h>
#include <game/ui/InputHandler.h>
#include <game/ui/LayoutUIControl.h>

class RadioButtonControl : public LayoutUIControl {
public:

    class OptionButton : public LayoutUIControl {
    public:
        virtual ~OptionButton();
        virtual void init();
        virtual void calc();

        virtual const TypeInfo* getTypeInfo() const;
        virtual const char* getTypeName() const;

        nw4r::lyt::Pane* fuchiPattern;
        nw4r::lyt::Pane* colorBase;
    };
    size_assert(OptionButton, 0x17C);

    virtual ~RadioButtonControl();
    virtual void init();
    virtual void calc();

    virtual const TypeInfo* getTypeInfo() const;
    virtual const char* getTypeName() const;

    virtual void vf_3c();
    virtual void vf_40();

    InputHandler<RadioButtonControl> onSelect;
    InputHandler<RadioButtonControl> onDeselect;
    InputHandler<RadioButtonControl> _19C;
    InputHandler<RadioButtonControl> onForwardPress;
    InputHandler<RadioButtonControl> onRightPress;
    InputHandler<RadioButtonControl> onLeftPress;
    u32 playerFlags;

    InputHandler<OptionButton>* onSelectedChangeHandler;
    InputHandler<OptionButton>* _1F4;
    InputHandler<OptionButton>* _1F8;
    InputHandler<OptionButton>* setSelectChangedHandler;

    s32 buttonCount;
    s32 prevSelected;
    s32 currSelected;
    s32 index;

    ControlInputManager inputManager;
    OptionButton* buttons;
};
size_assert(RadioButtonControl, 0x298);
