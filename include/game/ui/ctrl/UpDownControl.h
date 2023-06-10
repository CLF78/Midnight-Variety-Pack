#include <kamek.h>
#include <game/ui/ControlInputManager.h>
#include <game/ui/InputHandler.h>
#include <game/ui/LayoutUIControl.h>

class UpDownAnimator;

class UpDownButton : public LayoutUIControl {
public:
    UpDownButton();
    virtual ~UpDownButton();
    virtual void init();
    virtual void calc();

    virtual const TypeInfo* getTypeInfo() const;
    virtual const char* getTypeName() const;

    bool visibleMaybe;
    // 3 bytes padding

    nw4r::lyt::Pane* fuchiPattern;
    nw4r::lyt::Pane* colorBase;
    nw4r::lyt::Pane* colorArrow;
    u32 _184;
};
size_assert(UpDownButton, 0x188);

class UpDownControl : public LayoutUIControl {
public:
    UpDownControl();
    virtual ~UpDownControl();
    virtual void init();
    virtual void calc();

    virtual const TypeInfo* getTypeInfo() const;
    virtual const char* getTypeName() const;

    virtual void vf_3c();
    virtual void vf_40();

    void initLayout(int entryCount, int curSelected, const char* dirname, const char* filename,
                    const char* variant, const char* rightArrowCtr, const char* rightArrowVariant,
                    const char* leftArrowCtr, const char* leftArrowVariant, UpDownAnimator* animator,
                    u32 playerFlags, u32 holderFlags, bool unselectable, bool loop, bool _212);

    InputHandler0<UpDownControl, void> onSelect;
    InputHandler0<UpDownControl, void> onDeselect;
    InputHandler0<UpDownControl, void> onButtonSelect;
    InputHandler0<UpDownControl, void> onButtonDeselect;
    InputHandler0<UpDownControl, void> onFront;
    InputHandler0<UpDownControl, void> onRight;
    InputHandler0<UpDownControl, void> onLeft;

    u32 playerFlags;
    s32 curSelected;
    s32 entryCount;

    int id;

    bool loop;
    bool enabled;
    bool _212;
    // 1 byte padding

    UpDownAnimator* animator;
    ControlInputManager inputManager;
    InputHandler0<UpDownControl, void>* changeHandler;
    InputHandler0<UpDownControl, void>* frontHandler;
    InputHandler0<UpDownControl, void>* selectHandler;
    InputHandler0<UpDownControl, void>* deselectHandler;

    nw4r::lyt::Pane* fuchiPattern;
    nw4r::lyt::Pane* colorBase;

    UpDownButton buttons[2];
    s32 soundId;

    static UpDownControl* construct(void* buffer) {
        return new(buffer) UpDownControl;
    }

};
size_assert(UpDownControl, 0x5C8);

class UpDownAnimator {
public:
    void* vtable;
    InputHandler0<UpDownControl, void>* _4;
};
size_assert(UpDownAnimator, 0x8);
