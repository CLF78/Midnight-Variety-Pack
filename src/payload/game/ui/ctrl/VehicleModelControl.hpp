#pragma once
#include "LayoutUIControl.hpp"
#include <game/ui/page/Page.hpp>

class VehicleModelControl : public LayoutUIControl {
public:
    virtual ~VehicleModelControl();
    void setAnimationType(Page::PageId pageId);

    u8 hudSlotId;
    PAD(3);

    u32 mode;
    u8 animationType;
    PAD(3);

    float _180;
};
size_assert(VehicleModelControl, 0x184);
