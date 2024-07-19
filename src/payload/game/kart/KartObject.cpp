#include <game/kart/KartObject.hpp>
#include <game/kart/KartMove.hpp>
#include <game/system/RaceConfig.hpp>

REPLACE_STATIC KartObject* KartObject::Create(u8 playerIdx, int kart, int character, bool isBike){
    KartObject* object = REPLACED(playerIdx, kart, character, isBike);
    KartStats* stats = object->kartSettings->kartParam.stats;
    RaceConfig* raceConfig = RaceConfig::instance;

    // Check if engine class is more than 150cc
    // Multiply all standard acceleration stages if so
    u32 engineClass = raceConfig->raceScenario.settings.engineClass;
    if (engineClass <= RaceConfig::Settings::CC_150) {
        for (int i = 0; i < ARRAY_SIZE(stats->standard_acceleration_as); i++)
            stats->standard_acceleration_as[i] *= KartMove::speedModifiers[engineClass];
    }
    
    u8 chosenTransmission = raceConfig->raceScenario.players[playerIdx].transmission;
    switch(chosenTransmission){
        case RaceConfig::Player::TRANSMISSION_INSIDE:
            // If the vehicle isn't an inside-drifting bike, give the drift a buff
            if(stats->vehicleType != KartStats::INSIDE_BIKE){
                stats->automaticDrift += 0.002;
                stats->manualDrift += 0.002;
            }
            stats->vehicleType = KartStats::INSIDE_BIKE;
            break;
        case RaceConfig::Player::TRANSMISSION_OUTSIDE:
            // If the vehicle type is a Kart, these will already be set correctly
            if (isBike){
                stats->vehicleType = KartStats::OUTSIDE_BIKE;
                stats->outsideDriftTargetAngle = 45;
            }
            break;
        default:
            break;
    }
    return object;
}
