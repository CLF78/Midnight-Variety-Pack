#include <game/ui/LayoutUIControl.hpp>

class VehicleModelControl : public LayoutUIControl {
public:
    VehicleModelControl();
    virtual ~VehicleModelControl();
    void setAnimationType(u32 id);
    u8 hudSlotId;
    u32 mode;
    u8 animationType;
    float _180;
};
size_assert(VehicleModelControl, 0x184);