#include <kamek.h>
#include <game/kart/KartMove.h>
#include <game/kart/KartStats.h>
#include <game/system/RaceConfig.h>

// Update acceleration stats
kmBranchDefCpp(0x80592408, NULL, KartStats*, KartStats* self) {

    // Check if engine class is more than 150cc, if not bail
    u32 engineClass = RaceConfig::instance->raceScenario.settings.engineClass;
    if (engineClass <= RaceConfig::Settings::CC_150)
        return self;

    // Multiply all standard acceleration stages
    for (int i = 0; i < ARRAY_SIZE(self->standard_acceleration_as); i++)
        self->standard_acceleration_as[i] *= KartMove::speedModifiers[engineClass];

    // Return modified structure
    return self;
}
