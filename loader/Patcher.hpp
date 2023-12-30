#include <common/Common.hpp>
#include <Loader.hpp>

namespace Patcher {

enum PatchCommand {
    Addr32 = 1,
    Addr16Lo = 4,
    Addr16Hi = 5,
    Addr16Ha = 6,
    Rel24 = 10,
    Write32 = 32,
    Write16 = 33,
    Write8 = 34,
    Branch = 64,
    BranchLink = 65,
};

void ApplyPatches(const Loader::Functions* funcs, const u32* input, const u32* inputEnd, const u32 text);

inline void CacheInvalidateAddress(const void* address) {
    __dcbst(address, 0);
    __sync();
    __icbi(address);
}

inline u32 ResolveAddress(u32 text, u32 address) {
    return (address & 0x80000000) ? address : text + address;
}

} // namespace Patcher
