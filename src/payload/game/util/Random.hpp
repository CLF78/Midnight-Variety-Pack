#pragma once

class Random {
public:
    Random();
    explicit Random(const u32* seedValue); // custom constructor
    virtual ~Random();

    void initialize();
    u32 nextU32(u32 range);

    PAD(4);
    u64 x;
    u64 seed;
};
size_assert(Random, 0x18);
