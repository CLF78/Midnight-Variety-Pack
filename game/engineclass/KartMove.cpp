#include <kamek.h>
#include <game/system/RaceConfig.h>

static const float speedModifiers[] = {
    CC_50_SPEED_MODIFIER,
    CC_100_SPEED_MODIFIER,
    CC_150_SPEED_MODIFIER,
    CC_200_SPEED_MODIFIER,
    CC_500_SPEED_MODIFIER,
    CC_BATTLE_SPEED_MODIFIER,
};

// Replace speed modifier array
kmCallDefCpp(0x805780A4, const float*) {
    return speedModifiers;
}

// Apply the correct battle speed modifier
kmCallDefCpp(0x805780EC, float) {
    return speedModifiers[RaceConfig::Settings::CC_BATTLE];
}

// Glue code
kmWrite32(0x805780F0, 0xFC000890);
