#include <common/Common.hpp>
#include <game/ui/page/MenuPage.hpp>

class ClassSelectPage : public MenuPage {
public:
    virtual ~ClassSelectPage();

    virtual void onInit();
    virtual void onActivate();
    virtual void onDeactivate();
    virtual void afterCalc();

    RUNTIME_TYPE_INFO_NODECL;

    virtual void onSelectChange(PushButton* button, int playerId);
    virtual int getActivePlayers();
    virtual int getActiveControllers();
    virtual MenuInputManager* getInputManager();

    virtual void onStartPressed(int playerId);

    virtual PushButton* loadButton(int buttonIdx);
    virtual UIControl* loadLayout(u32 layoutIdx);
    virtual void setupButton(PushButton* button);

    bool switchModeOff;
    LayoutUIControl switchButton;

    static u32 engineClasses[6];
};
size_assert(ClassSelectPage, 0x83C);
