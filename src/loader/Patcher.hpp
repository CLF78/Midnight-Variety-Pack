#pragma once
#include <revolution/os/OSCache.h>

namespace Patcher {

enum PatchCommand {
    Addr32     = 1,
    Addr16Lo   = 4,
    Addr16Hi   = 5,
    Addr16Ha   = 6,
    Rel24      = 10,
    Write32    = 32,
    Write16    = 33,
    Write8     = 34,
    WriteArea  = 35,
    Branch     = 64,
    BranchLink = 65,
};

void ApplyPatches(const u32* input, const u32* inputEnd, u32 text);

inline void CacheInvalidateAddress(const void* address, u32 size) {
    if (size <= sizeof(u32)) {
        __dcbst(address, 0);
        __sync();
        __icbi(address);
    } else {
        __flush_cache((void*)address, size);
    }
}

inline u32 ResolveAddress(u32 text, u32 address) {
    return (address & 0x80000000) ? address : text + address;
}

} // namespace Patcher
