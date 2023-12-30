#include <common/Common.hpp>
#include <game/kart/KartObjectProxy.hpp>
#include <game/system/RaceConfig.hpp>
#include <revolution/mtx.h>

class KartMove : public KartObjectProxy {
public:
    virtual ~KartMove();

    // Actually virtual but cannot be arsed
    void init();
    void activateMega();

    float speedMultiplier;
    float baseSpeed;
    float softSpeedLimit;
    float _1C;

    float speed;
    float lastSpeed;
    float _28;

    float hardSpeedLimit;

    float acceleration;
    float draftMultiplier;

    u8 unk[0xCC-0x38];

    int hopStickX;

    u8 unk2[0x1F4 - 0xD0];

    VEC3 _1F4;

    u8 unk3[0x2C4 - 0x200];

    static const float speedModifiers[RaceConfig::Settings::CC_COUNT + 1];
    static float kartSpeedLimit;
    static float bulletSpeedLimit;

    static float minimumDriftThreshold;
};
size_assert(KartMove, 0x2C4);
