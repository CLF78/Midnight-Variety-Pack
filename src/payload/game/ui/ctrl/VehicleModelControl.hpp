#include <common/Common.hpp>
#include <game/ui/ctrl/LayoutUIControl.hpp>
#include <game/ui/page/Page.hpp>

class VehicleModelControl : public LayoutUIControl {
public:
    virtual ~VehicleModelControl();
    void setAnimationType(Page::PageId pageId);

    u8 hudSlotId;
    u32 mode;
    u8 animationType;
    float _180;
};
size_assert(VehicleModelControl, 0x184);
