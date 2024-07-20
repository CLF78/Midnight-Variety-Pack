#include <common/Common.hpp>
#include <game/ui/ControlInputManager.hpp>
#include <game/ui/InputHandler.hpp>
#include <game/ui/LayoutUIControl.hpp>

class SheetSelectButton : public LayoutUIControl {
public:
    virtual ~SheetSelectButton();

    virtual void init();
    virtual void calc();
    virtual void solveAnim(Element* element, float frame);

    RUNTIME_TYPE_INFO_NODECL;
    RUNTIME_TYPENAME_INFO_NODECL;

    void onDeselect(u32 localPlayerId);

    ControlInputManager inputManager;
    InputHandler1<SheetSelectButton, void, u32> onScrollHandler;
    InputHandler1<SheetSelectButton, void, u32> onDeselectHandler;
    InputHandler1<SheetSelectButton, void, u32> onSelectHandler;

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

    RUNTIME_TYPE_INFO_NODECL;
    RUNTIME_TYPENAME_INFO_NODECL;

    virtual void vf_38();
    virtual void onRightSelected();
    virtual void onLeftSelected();
    virtual void vf_44();

    void load(const char* dir, const char* rightFile, const char* rightVariant,
              const char* leftFile, const char* leftVariant, u32 playerFlags, bool r10,
              bool pointerOnly);

    void configure(bool leftEnabled, bool rightEnabled);

    InputHandler2<SheetSelectButton, void, SheetSelectControl*, u32>* onRightHandler;
    InputHandler2<SheetSelectButton, void, SheetSelectControl*, u32>* onLeftHandler;
    u32 playerFlags;
    u32 _A4;

    SheetSelectButton rightButton;
    SheetSelectButton leftButton;
};
size_assert(SheetSelectControl, 0x538);
