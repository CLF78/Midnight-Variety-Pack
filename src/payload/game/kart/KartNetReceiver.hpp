#pragma once
#include "KartObjectProxy.hpp"

class KartNetReceiver : public KartObjectProxy {
public:
    virtual ~KartNetReceiver();

    UNK(0x62 - 0x10);
    u16 halfpipeTimer; // Custom field, previously padding
    UNK(0xB4 - 0x64);
};
size_assert(KartNetReceiver, 0xB4);
