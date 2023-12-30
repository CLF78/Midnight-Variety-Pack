#include <common/Common.hpp>
#include <game/ui/ControlInputManager.hpp>
#include <game/ui/InputHandler.hpp>
#include <game/ui/LayoutUIControl.hpp>

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

    InputHandler0<RadioButtonControl, void> onSelect;
    InputHandler0<RadioButtonControl, void> onDeselect;
    InputHandler0<RadioButtonControl, void> _19C;
    InputHandler0<RadioButtonControl, void> onForwardPress;
    InputHandler0<RadioButtonControl, void> onRightPress;
    InputHandler0<RadioButtonControl, void> onLeftPress;
    u32 playerFlags;

    InputHandler0<OptionButton, void>* onSelectedChangeHandler;
    InputHandler0<OptionButton, void>* _1F4;
    InputHandler0<OptionButton, void>* _1F8;
    InputHandler0<OptionButton, void>* setSelectChangedHandler;

    s32 buttonCount;
    s32 prevSelected;
    s32 currSelected;
    s32 index;

    ControlInputManager inputManager;
    OptionButton* buttons;
};
size_assert(RadioButtonControl, 0x298);
