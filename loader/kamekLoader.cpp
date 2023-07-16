#include <rvl/dvd/dvd.h>
#include <rvl/os/OS.h>
#include <rvl/os/OSCache.h>
#include <stdlib/string.h>
#include "kamekLoader.h"

#define kCommandHandler(name) \
    static inline const u32* kHandle##name(const u32* input, u32 text, u32 address)
#define kDispatchCommand(name) \
    case k##name: input = kHandle##name(input, text, address); break

kCommandHandler(Addr32) {
    u32 target = resolveAddress(text, *input++);
    *(u32*)address = target;
    return input;
}

kCommandHandler(Addr16Lo) {
    u32 target = resolveAddress(text, *input++);
    *(u16*)address = target & 0xFFFF;
    return input;
}

kCommandHandler(Addr16Hi) {
    u32 target = resolveAddress(text, *input++);
    *(u16*)address = target >> 16;
    return input;
}

kCommandHandler(Addr16Ha) {
    u32 target = resolveAddress(text, *input++);
    *(u16*)address = target >> 16;
    if (target & 0x8000)
        *(u16*)address++;
    return input;
}

kCommandHandler(Rel24) {
    u32 target = resolveAddress(text, *input++);
    u32 delta = target - address;
    *(u32*)address &= 0xFC000003;
    *(u32*)address |= (delta & 0x3FFFFFC);
    return input;
}

kCommandHandler(Write32) {
    u32 value = *input++;
    *(u32*)address = value;
    return input;
}

kCommandHandler(Write16) {
    u32 value = *input++;
    *(u16*)address = value & 0xFFFF;
    return input;
}

kCommandHandler(Write8) {
    u32 value = *input++;
    *(u8*)address = value & 0xFF;
    return input;
}

kCommandHandler(CondWritePointer) {
    u32 target = resolveAddress(text, *input++);
    u32 original = *input++;
    if (*(u32*)address == original)
        *(u32*)address = target;
    return input;
}

kCommandHandler(CondWrite32) {
    u32 value = *input++;
    u32 original = *input++;
    if (*(u32*)address == original)
        *(u32*)address = value;
    return input;
}

kCommandHandler(CondWrite16) {
    u32 value = *input++;
    u32 original = *input++;
    if (*(u16*)address == (original & 0xFFFF))
        *(u16*)address = value & 0xFFFF;
    return input;
}

kCommandHandler(CondWrite8) {
    u32 value = *input++;
    u32 original = *input++;
    if (*(u8*)address == (original & 0xFF))
        *(u8*)address = value & 0xFF;
    return input;
}

kCommandHandler(Branch) {
    *(u32*)address = 0x48000000;
    return kHandleRel24(input, text, address);
}

kCommandHandler(BranchLink) {
    *(u32*)address = 0x48000001;
    return kHandleRel24(input, text, address);
}

void kamekError(const loaderFunctions* funcs, const char* str) {
    u32 fg = 0xFFFFFFFF, bg = 0;
    funcs->OSFatal(&fg, &bg, str);
}

void loadKamekBinary(const loaderFunctions* funcs, u32 binary, u32 binaryLength) {
    const KBHeader* header = (const KBHeader*)binary;

    // Check magic
    if (header->magic1 != 'Kame' || header->magic2 != 'k\0')
        kamekError(funcs, "FATAL ERROR: Corrupted file, please check your game's Kamek files");

    // Check version number
    if (header->version != 2)
        kamekError(funcs, "FATAL ERROR: Incompatible file version, please upgrade your Kamek Loader");

    // Obtain data from the header and report it
    u32 codeSize = header->codeSize;
    u32 bssSize = header->bssSize;
    u32 ctorStart = header->ctorStart;
    u32 ctorEnd = header->ctorEnd;
    funcs->OSReport("header: bssSize=%u, codeSize=%u, ctors=%u-%u\n",
        bssSize, codeSize, ctorStart, ctorEnd);

    // Allocate text section buffer, bail on failure
    u32 textSize = codeSize + bssSize;
    u32 text = (u32)(funcs->rkSystem->EGGSystem->alloc(textSize, 0x20));
    if (!text)
        kamekError(funcs, "FATAL ERROR: Out of code memory");

    // Get input and input end pointers
    const u32* input = (u32*)(binary + sizeof(KBHeader));
    const u32* inputEnd = (u32*)(binary + binaryLength);
    u8* output = (u8*)text;

    // Create text + bss sections
    memcpy(output, input, codeSize);
    memset(output + codeSize, 0, bssSize);
    __flush_cache(output, textSize);
    input += (codeSize / sizeof(u32));

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
            address += text; // Relative

        switch (cmd) {
            kDispatchCommand(Addr32);
            kDispatchCommand(Addr16Lo);
            kDispatchCommand(Addr16Hi);
            kDispatchCommand(Addr16Ha);
            kDispatchCommand(Rel24);
            kDispatchCommand(Write32);
            kDispatchCommand(Write16);
            kDispatchCommand(Write8);
            kDispatchCommand(CondWritePointer);
            kDispatchCommand(CondWrite32);
            kDispatchCommand(CondWrite16);
            kDispatchCommand(CondWrite8);
            kDispatchCommand(Branch);
            kDispatchCommand(BranchLink);
            default:
                funcs->OSReport("Unknown command: %d\n", cmd);
        }

        cacheInvalidateAddress((const void*)address);
    }

    __sync();
    __isync();

    typedef void (*Func)();
    for (Func* f = (Func*)(text + ctorStart); f < (Func*)(text + ctorEnd); f++) {
        (*f)();
    }
}

void loadKamekBinaryFromDisc(const loaderFunctions* funcs, const char* path) {
    funcs->OSReport("{Kamek by Treeki}\nLoading Kamek binary '%s'...\n", path);

    // Locate file
    int entrynum = funcs->DVDConvertPathToEntrynum(path);
    if (entrynum < 0) {
        char err[64];
        funcs->snprintf(err, sizeof(err), "FATAL ERROR: Failed to locate file on the disc: %s", path);
        kamekError(funcs, err);
    }

    // Open it
    DVDFileInfo fileInfo;
    if (!funcs->DVDFastOpen(entrynum, &fileInfo))
        kamekError(funcs, "FATAL ERROR: Failed to open file!");

    funcs->OSReport("DVD file located: addr=%p, size=%d\n", fileInfo.startAddr, fileInfo.length);

    // Allocate read buffer
    u32 roundedLength = OSRoundUp32B(fileInfo.length);
    void* buffer = funcs->rkSystem->EGGSystem->alloc(roundedLength, -0x20);
    if (!buffer)
        kamekError(funcs, "FATAL ERROR: Out of file memory");

    // Read the file and close it
    funcs->DVDReadPrio(&fileInfo, buffer, roundedLength, 0, 2);
    funcs->DVDClose(&fileInfo);

    // Apply the data
    loadKamekBinary(funcs, (u32)buffer, fileInfo.length);

    // Free the buffer and return
    funcs->rkSystem->EGGSystem->free(buffer);
    funcs->OSReport("All done!\n");
}
