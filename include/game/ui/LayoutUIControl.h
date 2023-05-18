#include <kamek.h>
#include <game/ui/UIControl.h>
#include <game/ui/UIAnimator.h>
#include <game/ui/Layout.h>
#include <game/ui/MessageGroup.h>
#include <game/ui/MessageInfo.h>

class LayoutUIControl : public UIControl {
public:
    virtual ~LayoutUIControl();

    virtual void init();
    virtual void calc();
    virtual void draw(int drawPass);

    virtual const TypeInfo* getTypeInfo() const;
    virtual const char* getTypeName() const;
    virtual void vf_30();

    virtual void loadResources(const char* arcName, const char* layoutName);

    void setMsgId(u32 bmgId, MessageInfo* info);
    void setMatIcon(const char* materialName, const char* iconPaneName);
    void resetText();

    UIAnimator animator;
    MainLayout mainLayout;
    MessageGroup specificMessageGroup;
    MessageGroup commonMessageGroup;
    PictureSourceLayout* pictureSourceLayout;
    nw4r::lyt::Pane* mainPane;
};
size_assert(LayoutUIControl, 0x174);
