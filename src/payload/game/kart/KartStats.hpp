#pragma once
#include <common/Common.hpp>

class KartStats {
public:

    enum WheelCount {
        WHEELS_4,
        WHEELS_2,
        WHEELS_2_QUACKER,
        WHEELS_3,
    };

    enum DriftType {
        KART,
        OUTSIDE_BIKE,
        INSIDE_BIKE,
    };

    enum WeightClass {
        LIGHT,
        MEDIUM,
        HEAVY,
    };

    struct RotatingItemObjParams {
        float zRadius;
        float xRadius;
        float yDist;
        float zDist;
    };

    int wheelCount;
    int vehicleType;
    int weightClass;

    float _C;
    float weight;
    float bumpDeviationLevel;
    float baseSpeed;
    float handlingSpeedMultiplier;
    float tilt;
    float standard_acceleration_as[4];
    float standard_acceleration_ts[3];
    float drift_acceleration_as[2];
    float drift_acceleration_ts;
    float manualHandling;
    float autoHandling;
    float handlingReactivity;
    float manualDrift;
    float automaticDrift;
    float driftReactivity;
    float outsideDriftTargetAngle;
    float outsideDriftDecrement;

    int mtDuration;
    float speedFactors[32];
    float handlingFactors[32];

    RotatingItemObjParams rotatingItemObjParams;
    float vertical_tilt;
    float megaScale;
    float axleDistance;
};
size_assert(KartStats, 0x18C);
