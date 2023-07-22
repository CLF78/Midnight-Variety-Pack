#include <kamek.h>
#include <game/kart/KartObjectProxy.h>
#include <game/system/RaceConfig.h>

class KartMove : public KartObjectProxy {
public:
    virtual ~KartMove();

    // Actually virtual but cannot be arsed
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

    u8 unk2[0x2C4 - 0xD0];

    static const float speedModifiers[RaceConfig::Settings::CC_COUNT + 1];
};
size_assert(KartMove, 0x2C4);
