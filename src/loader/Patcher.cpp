#include <common/Common.hpp>
#include <platform/string.h>
#include <Patcher.hpp>

namespace Patcher {

void ApplyPatches(const u32* input, const u32* inputEnd, const u32 text) {
    LOG_DEBUG("Applying patches...");

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
        u32 size = 0;
        u32* placeholder, *originalInstr;
        switch (cmd) {

            case Addr32:
                target = ResolveAddress(text, *input++);
                *(u32*)address = target;
                CacheInvalidateAddress((const void*)address, sizeof(u32));
                break;

            case Addr16Lo:
                target = ResolveAddress(text, *input++);
                *(u16*)address = target & 0xFFFF;
                CacheInvalidateAddress((const void*)address, sizeof(u16));
                break;

            case Addr16Hi:
                target = ResolveAddress(text, *input++);
                *(u16*)address = target >> 16;
                CacheInvalidateAddress((const void*)address, sizeof(u16));
                break;

            case Addr16Ha:
                target = ResolveAddress(text, *input++);
                *(u16*)address = target >> 16;
                if (target & 0x8000)
                    (*(u16*)address)++;
                CacheInvalidateAddress((const void*)address, sizeof(u16));
                break;

            case Rel24:
                target = ResolveAddress(text, *input++);
                delta = target - address;
                *(u32*)address &= 0xFC000003;
                *(u32*)address |= (delta & 0x3FFFFFC);
                CacheInvalidateAddress((const void*)address, sizeof(u32));
                break;

            case Write32:
                value = *input++;
                *(u32*)address = value;
                CacheInvalidateAddress((const void*)address, sizeof(u32));
                break;

            case Write16:
                value = *input++;
                *(u16*)address = value & 0xFFFF;
                CacheInvalidateAddress((const void*)address, sizeof(u16));
                break;

            case Write8:
                value = *input++;
                *(u8*)address = value & 0xFF;
                CacheInvalidateAddress((const void*)address, sizeof(u8));
                break;

            case WriteArea:
                target = ResolveAddress(text, *input++);
                size = *input++;
                memcpy((void*)address, (void*)target, size);
                CacheInvalidateAddress((const void*)address, size);
                break;

            case Branch:
                target = ResolveAddress(text, *input++);

                // Hack for the replace keyword
                placeholder = (u32*)target;
                originalInstr = (u32*)address;
                if (*placeholder == 0x78787878) {
                    *placeholder = originalInstr[-1];
                    CacheInvalidateAddress(placeholder, sizeof(u32));
                    break;
                }

                // Regular behaviour
                delta = target - address;
                *(u32*)address = 0x48000000 | (delta & 0x3FFFFFC);
                CacheInvalidateAddress((const void*)address, sizeof(u32));
                break;

            case BranchLink:
                target = ResolveAddress(text, *input++);
                delta = target - address;
                *(u32*)address = 0x48000001 | (delta & 0x3FFFFFC);
                CacheInvalidateAddress((const void*)address, sizeof(u32));
                break;

            default:
                LOG_WARN("Unknown patch type: %d", cmd);
        }
    }
}

} // namespace Patcher
