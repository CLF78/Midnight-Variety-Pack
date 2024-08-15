#include "KartMove.hpp"
#include "KartObject.hpp"
#include <game/system/RaceConfig.hpp>

/////////////////////////////////////////////////
// Custom Engine Classes / Transmission Select //
/////////////////////////////////////////////////

// Apply engine class and transmission changes
REPLACE_STATIC KartObject* KartObject::Create(u8 playerIdx, int kart, int character, bool isBike) {

    // Get structures
    KartObject* object = REPLACED(playerIdx, kart, character, isBike);
    KartStats* stats = object->kartSettings->kartParam->stats;
    RaceConfig* raceConfig = RaceConfig::instance;

    // Check if engine class is more than 150cc
    // Multiply all standard acceleration stages if so
    const u32 engineClass = raceConfig->raceScenario.settings.engineClass;
    if (engineClass <= RaceConfig::Settings::CC_150) {
        for (u32 i = 0; i < ARRAY_SIZE(stats->standard_acceleration_as); i++)
            stats->standard_acceleration_as[i] *= KartMove::speedModifiers[engineClass];
    }

    // Apply transmission changes
    switch(raceConfig->raceScenario.players[playerIdx].transmission) {

        // If the vehicle isn't already an inside-drifting bike, buff the drift stats as well
        case RaceConfig::Player::TRANSMISSION_INSIDE:
            if (stats->vehicleType != KartStats::INSIDE_BIKE) {
                stats->automaticDrift += 0.002;
                stats->manualDrift += 0.002;
            }
            stats->vehicleType = KartStats::INSIDE_BIKE;
            break;

        // If the vehicle type is a bike, ensure the drift angle is set
        case RaceConfig::Player::TRANSMISSION_OUTSIDE:
            if (isBike) {
                stats->vehicleType = KartStats::OUTSIDE_BIKE;
                stats->outsideDriftTargetAngle = 45;
            }
            break;

        // Do not change the stats for the default transmission
        default:
            break;
    }

    // Return object to match original function return type
    return object;
}
