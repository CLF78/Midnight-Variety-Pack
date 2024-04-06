#include <common/Common.hpp>
#include <game/kart/KartMove.hpp>
#include <game/kart/KartParam.hpp>
#include <game/race/RaceGlobals.hpp>
#include <game/system/RaceConfig.hpp>
#include <wiimmfi/Reporting.hpp>

///////////////////////////
// Custom Engine Classes //
///////////////////////////

// Update acceleration stats
REPLACE_STATIC KartStats* KartParam::computeStats(int vehicle, int character) {

    // Get stats
    KartStats* stats = REPLACED(vehicle, character);

    // Check if engine class is more than 150cc
    // Multiply all standard acceleration stages if so
    u32 engineClass = RaceConfig::instance->raceScenario.settings.engineClass;
    if (engineClass <= RaceConfig::Settings::CC_150) {
        for (int i = 0; i < ARRAY_SIZE(stats->standard_acceleration_as); i++)
            stats->standard_acceleration_as[i] *= KartMove::speedModifiers[engineClass];
    }

    // Return stats
    return stats;
}

///////////////////////
// Wiimmfi Telemetry //
///////////////////////

// Report Common.szs subfile hashes for potential cheating
// Credits: Wiimmfi
REPLACE_STATIC void KartParam::init() {
    REPLACED();
    if (RaceGlobals::isOnlineRace)
        Wiimmfi::Reporting::ReportCommonSubfiles();
}
