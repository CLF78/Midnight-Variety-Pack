#include <common/Common.hpp>
#include <game/kart/KartMove.hpp>
#include <game/kart/KartStats.hpp>
#include <game/system/RaceConfig.hpp>

///////////////////////////////////////
// Patches for Custom Engine Classes //
///////////////////////////////////////

// Update acceleration stats
kmBranchDefCpp(0x80592408, NULL, KartStats*, KartStats* self) {

    // Check if engine class is more than 150cc
    // Multiply all standard acceleration stages if so
    u32 engineClass = RaceConfig::instance->raceScenario.settings.engineClass;
    if (engineClass <= RaceConfig::Settings::CC_150) {
        for (int i = 0; i < ARRAY_SIZE(self->standard_acceleration_as); i++)
            self->standard_acceleration_as[i] *= KartMove::speedModifiers[engineClass];
    }

    // Return structure since we're hooking a constructor
    return self;
}
