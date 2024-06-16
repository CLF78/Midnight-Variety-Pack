#include <common/Common.hpp>

class Random {
public:
    Random();
    Random(u32* seedValue); // custom constructor
    virtual ~Random();

    void initialize();
    u32 nextU32(u32 range);

    u64 x;
    u64 seed;
};
size_assert(Random, 0x18);
