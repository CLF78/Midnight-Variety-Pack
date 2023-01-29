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
    InputHandler<PushButton> onSelectHandlerObj;
    InputHandler<PushButton> onDeselectHandlerObj;
    InputHandler<PushButton> onClickHandlerObj;
    InputHandler<PushButton>* onSelectHandler;
    InputHandler<PushButton>* onDeselectHandler;
    InputHandler<PushButton>* onClickHandler;

    int buttonId;
    int playerCount;

    Pane* fuchiPane;
    Pane* colorBasePane;

    int selectionSound;
};
size_assert(PushButton, 0x254);
