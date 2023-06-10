#include <kamek.h>
#include <game/ui/ControlInputManager.h>
#include <game/ui/InputHandler.h>
#include <game/ui/LayoutUIControl.h>
#include <stdlib/new.h>

class PushButton : public LayoutUIControl {
public:
    PushButton();
    virtual ~PushButton();

    virtual void init();
    virtual void calc();

    virtual const TypeInfo* getTypeInfo() const;
    virtual const char* getTypeName() const;

    virtual void onButtonInit();
    virtual void onSelect();
    virtual void onDeselect();
    virtual void onClick();

    void initLayout(const char* dirname, const char* filename, const char* variant, u32 playerFlags,
                    bool param_6, bool inaccessible);

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

    static PushButton* construct(void* buffer) {
        return new(buffer) PushButton;
    }
};
size_assert(PushButton, 0x254);
