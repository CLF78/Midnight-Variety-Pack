#include "Random.hpp"

Random::Random(u32* seedValue) {

    // Replace the seed if provided
    if (seedValue) {
        x = *seedValue;
        seed = *seedValue;
    } else {
        initialize();
    }
}
