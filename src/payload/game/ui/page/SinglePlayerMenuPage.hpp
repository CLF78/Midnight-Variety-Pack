#include <common/Common.hpp>
#include <game/ui/page/MenuPage.hpp>

class SinglePlayerMenuPage : public MenuPage {
public:
    enum ButtonID {
        GP_BUTTON,
        TT_BUTTON,
        VS_BUTTON,
        BT_BUTTON,
        MR_BUTTON,
        BUTTON_COUNT,
    };
};
size_assert(SinglePlayerMenuPage, 0x6C4);
