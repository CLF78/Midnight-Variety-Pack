#pragma once
#include "UIControl.hpp"
#include <game/ui/Layout.hpp>
#include <game/ui/MessageGroup.hpp>
#include <game/ui/MessageInfo.hpp>
#include <game/ui/UIAnimator.hpp>

class LayoutUIControl : public UIControl {
public:
    LayoutUIControl();
    virtual ~LayoutUIControl();

    virtual void init();
    virtual void calc();
    virtual void draw(int drawPass);

    RUNTIME_TYPE_INFO_NODECL;
    virtual void vf_30();

    virtual void loadResources(const char* arcName, const char* layoutName);

    void setMovieCrop(const char* pane, float top, float bottom, float left, float right);
    void setText(u32 bmgId, MessageInfo* info = nullptr);
    void setMatIcon(const char* materialName, const char* iconPaneName);
    void setMatText(const char* materialName, u32 bmgId, MessageInfo* info = nullptr);
    void setPaneVisible(const char* pane, bool visible);
    void resetText();

    UIAnimator animator;
    MainLayout mainLayout;
    MessageGroup specificMessageGroup;
    MessageGroup commonMessageGroup;
    PictureSourceLayout* pictureSourceLayout;
    nw4r::lyt::Pane* mainPane;
};
size_assert(LayoutUIControl, 0x174);
