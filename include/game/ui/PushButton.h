#include <kamek.h>
#include <game/ui/ControlInputManager.h>
#include <game/ui/InputHandler.h>
#include <game/ui/LayoutUIControl.h>

class PushButton : public LayoutUIControl {
public:
    virtual ~PushButton();

    virtual void init();
    virtual void calc();

    virtual const TypeInfo* getTypeInfo() const;
    virtual const char* getTypeName() const;

    virtual void onButtonInit();
    virtual void onSelect();
    virtual void onDeselect();
    virtual void onClick();

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
