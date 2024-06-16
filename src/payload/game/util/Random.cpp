#include <common/Common.hpp>
#include <game/util/Random.hpp>

Random::Random(u32* seedValue) {

    // Replace the seed if provided
    if (seedValue) {
        x = *seedValue;
        seed = *seedValue;
    } else {
        initialize();
    }
}
