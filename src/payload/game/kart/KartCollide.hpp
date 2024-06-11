#include <common/Common.hpp>
#include <game/kart/KartObjectProxy.hpp>

class KartCollideBase {
public:
    virtual void processVehicleBodyCollision(void* collisionData, void* hitbox, void* unk, u32* kclFlags) = 0;
    virtual void processWheelCollision(void* collisionData, void* hitbox, void* unk, u32* kclFlags) = 0;
    virtual ~KartCollideBase() = 0;
};

class KartCollide : public KartCollideBase, KartObjectProxy {
public:
    virtual void processVehicleBodyCollision(void* collisionData, void* hitbox, void* unk, u32* kclFlags);
    virtual void processWheelCollision(void* collisionData, void* hitbox, void* unk, u32* kclFlags);
    virtual ~KartCollide();

    void activateOob(bool unk, u32* kclFlags, bool unk2, bool unk3);

    u8 unk[0x6C-0x10];
    s16 respawnArea;
    u8 unk2[0x74-0x6E];
};
size_assert(KartCollide, 0x74);
