#include <common/Common.hpp>
#include <platform/string.h>
#include <Patcher.hpp>

namespace Patcher {

void ApplyPatches(const Loader::Functions* funcs, const u32* input, const u32* inputEnd, const u32 text) {

   while (input < inputEnd) {

        // Get command header
        u32 cmdHeader = *input++;

        // Get command
        u8 cmd = cmdHeader >> 24;

        // Get address
        u32 address = cmdHeader & 0xFFFFFF;
        if (address == 0xFFFFFE)
            address = *input++; // Absolute
        else
            address = text + address; // Relative

        // Apply patch
        u32 target, value, delta;
        switch (cmd) {

            case Addr32:
                target = ResolveAddress(text, *input++);
                *(u32*)address = target;
                break;

            case Addr16Lo:
                target = ResolveAddress(text, *input++);
                *(u16*)address = target & 0xFFFF;
                break;

            case Addr16Hi:
                target = ResolveAddress(text, *input++);
                *(u16*)address = target >> 16;
                break;

            case Addr16Ha:
                target = ResolveAddress(text, *input++);
                *(u16*)address = target >> 16;
                if (target & 0x8000)
                    (*(u16*)address)++;
                break;

            case Rel24:
                target = ResolveAddress(text, *input++);
                delta = target - address;
                *(u32*)address &= 0xFC000003;
                *(u32*)address |= (delta & 0x3FFFFFC);
                break;

            case Write32:
                value = *input++;
                *(u32*)address = value;
                break;

            case Write16:
                value = *input++;
                *(u16*)address = value & 0xFFFF;
                break;

            case Write8:
                value = *input++;
                *(u8*)address = value & 0xFF;
                break;

            case WriteArea:
                target = ResolveAddress(text, *input++);
                value = *input++;
                memcpy((void*)address, (void*)target, value);
                break;

            case Branch:
                target = ResolveAddress(text, *input++);
                delta = target - address;
                *(u32*)address = 0x48000000 | (delta & 0x3FFFFFC);
                break;

            case BranchLink:
                target = ResolveAddress(text, *input++);
                delta = target - address;
                *(u32*)address = 0x48000001 | (delta & 0x3FFFFFC);
                break;

            default:
                funcs->OSReport("Unknown patch type: %d\n", cmd);
        }

        CacheInvalidateAddress((const void*)address, (cmd == WriteArea) ? value : 0);
   }
}

} // namespace Patcher
