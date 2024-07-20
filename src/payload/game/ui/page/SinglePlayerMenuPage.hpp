#include <common/Common.hpp>
#include <game/ui/page/MenuPage.hpp>

class SinglePlayerMenuPage : public MenuPage {
public:
    virtual ~SinglePlayerMenuPage();

    virtual void onInit();
    virtual void onActivate();
    virtual void onDeactivate();
    virtual void afterCalc();

    RUNTIME_TYPE_INFO_NODECL;

    virtual void onSelectChange(PushButton* button, int playerId);
    virtual int getActivePlayers();
    virtual int getActiveControllers();
    virtual MenuInputManager* getInputManager();

    virtual PushButton* loadButton(int buttonIdx);
    virtual UIControl* loadLayout(u32 layoutIdx);
    virtual void setupButton(PushButton* button);

    void onButtonClick(PushButton* button, u32 hudSlotId);

    enum ButtonID {
        GP_BUTTON,
        TT_BUTTON,
        VS_BUTTON,
        BT_BUTTON,
        MR_BUTTON,
        BUTTON_COUNT,
    };
};
size_assert(SinglePlayerMenuPage, sizeof(MenuPage));
