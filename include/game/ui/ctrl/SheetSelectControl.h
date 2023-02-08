#include <kamek.h>
#include <stdlib/new.h>
#include <game/ui/ControlInputManager.h>
#include <game/ui/InputHandler.h>
#include <game/ui/LayoutUIControl.h>

class SheetSelectButton : public LayoutUIControl {
public:
    virtual ~SheetSelectButton();

    virtual void init();
    virtual void calc();
    virtual void solveAnim(Element* element, float frame);

    virtual const TypeInfo* getTypeInfo() const;
    virtual const char* getTypeName() const;

    ControlInputManager m_inputManager;
    InputHandler<SheetSelectButton> _1F8;
    InputHandler<SheetSelectButton> _20C;
    InputHandler<SheetSelectButton> onScrollHandler;

    int arrowSide;
    u32 playerFlags;
    bool enabled;

    nw4r::lyt::Pane* fuchiPane;
    nw4r::lyt::Pane* rootPane;
};
size_assert(SheetSelectButton, 0x248);

class SheetSelectControl : public UIControl {
public:
    SheetSelectControl();
    virtual ~SheetSelectControl();
    virtual void init();

    virtual const TypeInfo* getTypeInfo() const;
    virtual const char* getTypeName() const;

    virtual void vf_38();
    virtual void onRightSelected();
    virtual void onLeftSelected();
    virtual void vf_44();

    void load(const char *dir, const char *rightFile, const char *rightVariant,
            const char *leftFile, const char *leftVariant, u32 playerFlags, bool r10,
            bool pointerOnly);

    InputHandlerEx<SheetSelectButton, SheetSelectControl>* rightHandler;
    InputHandlerEx<SheetSelectButton, SheetSelectControl>* leftHandler;
    u32 playerFlags;
    u32 _A4;

    SheetSelectButton rightButton;
    SheetSelectButton leftButton;

    static SheetSelectControl* construct(void* buffer) {
        return new(buffer) SheetSelectControl;
    }
};
size_assert(SheetSelectControl, 0x538);
