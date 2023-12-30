#include <common/Common.hpp>
#include <game/ui/page/MenuPage.hpp>

class ClassSelectPage : public MenuPage {
public:
    bool switchModeOff;
    LayoutUIControl switchButton;

    static u32 engineClasses[6];
};
size_assert(ClassSelectPage, 0x83C);
