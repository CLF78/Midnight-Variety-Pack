#include <kamek.h>
#include <game/ui/UIControl.h>
#include <game/ui/UIAnimator.h>
#include <game/ui/Layout.h>
#include <game/ui/MessageGroup.h>

class PictureSourceLayout;
class Pane;

class LayoutUIControl : public UIControl {
public:
    virtual ~LayoutUIControl();

    virtual void init();
    virtual void calc();
    virtual void draw(int drawPass);

    virtual const TypeInfo* getTypeInfo() const;
    virtual const char* getTypeName() const;
    virtual void vf30();

    virtual void loadResources(const char* arcName, const char* layoutName);

    UIAnimator animator;
    MainLayout mainLayout;
    MessageGroup specificMessageGroup;
    MessageGroup commonMessageGroup;
    PictureSourceLayout* pictureSourceLayout;
    Pane* mainPane;
};
size_assert(LayoutUIControl, 0x174);
