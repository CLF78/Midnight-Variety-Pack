#pragma once
#include "Layout.hpp"

class UIAnimationGroup {
public:
    void setAnimation(u32 animationId, float frame);
};

class UIAnimator {
public:
    UIAnimationGroup* getGroup(u32 groupId);

    UIAnimationGroup* groups;
    u32 groupCount;
    nw4r::lyt::Pane* rootPane;
    MainLayout* mainLayout;
};
size_assert(UIAnimator, 0x10);
