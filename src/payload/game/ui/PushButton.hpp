#include <common/Common.hpp>
#include <game/ui/ControlInputManager.hpp>
#include <game/ui/InputHandler.hpp>
#include <game/ui/LayoutUIControl.hpp>

class PushButton : public LayoutUIControl {
public:
    PushButton();
    virtual ~PushButton();

    virtual void init();
    virtual void calc();

    RUNTIME_TYPE_INFO_NODECL;
    RUNTIME_TYPENAME_INFO_NODECL;

    virtual void onButtonInit();
    virtual void onSelect();
    virtual void onDeselect();
    virtual void onClick();

    void load(const char* dirname, const char* filename, const char* variant, u32 playerFlags,
                    bool param_6, bool inaccessible);
    void loadWithAnims(const char** anims, const char* dirname, const char* filename,
                       const char* variant, u32 playerFlags, bool param_6);

    bool isSelected();
    void selectDefault(u32 localPlayerId);
    void click(u32 localPlayerId);

    float getDelay();

    void setOnClickHandler(InputHandlerBase* handler, u8 unk);
    void setOnSelectHandler(InputHandlerBase* handler);

    ControlInputManager inputManager;
    InputHandler0<PushButton, void> onSelectHandlerObj;
    InputHandler0<PushButton, void> onDeselectHandlerObj;
    InputHandler0<PushButton, void> onClickHandlerObj;
    InputHandler0<PushButton, void>* onSelectHandler;
    InputHandler0<PushButton, void>* onDeselectHandler;
    InputHandler0<PushButton, void>* onClickHandler;

    int buttonId;
    int playerCount;

    nw4r::lyt::Pane* fuchiPane;
    nw4r::lyt::Pane* colorBasePane;

    int selectionSound;
};
size_assert(PushButton, 0x254);
