#include <common/Common.hpp>
#include <game/ui/page/Page.hpp>
#include <game/ui/ctrl/LayoutUIControl.hpp>

class DemoPage : public Page {
public:
    virtual ~DemoPage();

    virtual void onInit();
    RUNTIME_TYPE_INFO_NODECL;

    LayoutUIControl courseName;
    LayoutUIControl topText;
    MenuInputManager inputManager;
};
size_assert(DemoPage, 0x33C);
