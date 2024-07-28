#pragma once
#include <common/Common.hpp>

class UnlockFlags {
public:
    virtual ~UnlockFlags();
    bool get(u32 flag);

    u32 rawFlags[4];
};
