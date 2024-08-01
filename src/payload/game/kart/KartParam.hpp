#pragma once
#include "KartStats.hpp"

class KartParam {
public:
    static void init();
    static KartStats* computeStats(int vehicle, int character);

    KartStats* stats;
    void* bsp;
};
size_assert(KartParam, 0x8);
