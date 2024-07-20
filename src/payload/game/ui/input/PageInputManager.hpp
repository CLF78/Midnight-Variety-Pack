#include <common/Common.hpp>
#include <game/ui/input/ButtonInfo.hpp>
#include <game/ui/InputHandler.hpp>
#include <game/ui/ctrl/LayoutUIControl.hpp>
#include <game/ui/input/MenuInputManager.hpp>

class PageInputManager : public MenuInputManager {
public:
    InputHandler2<LayoutUIControl, void, u32, u32>* handlers[INPUT_COUNT];
    bool isTriggered[INPUT_COUNT];
    ButtonInfo buttoninfoArray[5];
};
size_assert(PageInputManager, 0x144);
