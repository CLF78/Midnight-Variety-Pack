#include <kamek.h>
#include <game/ui/Page.h>
#include <game/ui/LayoutUIControl.h>

class DemoPage : public Page {
public:
    virtual ~DemoPage();

    virtual void onInit();
    virtual TypeInfo* getTypeInfo() const;

    LayoutUIControl courseName;
    LayoutUIControl topText;
    MenuInputManager inputManager;
};
size_assert(DemoPage, 0x33C);
