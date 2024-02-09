#include <common/Common.hpp>
#include <game/kart/KartMove.hpp>
#include <game/kart/KartStats.hpp>
#include <game/race/RaceGlobals.hpp>
#include <game/system/RaceConfig.hpp>
#include <wiimmfi/Reporting.hpp>

///////////////////////////
// Custom Engine Classes //
///////////////////////////

// KartParam::ComputeStats() patch
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

///////////////////////
// Wiimmfi Telemetry //
///////////////////////

// KartParam::Init() patch
// Report Common.szs subfile hashes for potential cheating
// Credits: Wiimmfi
kmBranchDefCpp(0x80591B70, NULL, void) {
    if (RaceGlobals::isOnlineRace)
        Wiimmfi::Reporting::ReportCommonSubfiles();
}
