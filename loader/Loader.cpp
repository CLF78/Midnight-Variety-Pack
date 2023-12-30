#include <platform/string.h>
#include <revolution/dvd/dvd.h>
#include <revolution/os/OS.h>
#include <revolution/os/OSCache.h>
#include <revolution/os/OSLoMem.h>
#include <Loader.hpp>
#include <Patcher.hpp>

namespace Loader {

void Error(const Functions* funcs, const char* str) {
    u32 fg = 0xFFFFFFFF, bg = 0;
    funcs->OSFatal(&fg, &bg, str);
}

void Load(const Functions* funcs, u32 binary, u32 binaryLength) {
    const Header* header = (const Header*)binary;

    // Check magic
    if (header->magic1 != DEFAULT_MAGIC1 || header->magic2 != DEFAULT_MAGIC2)
        Error(funcs, "FATAL ERROR: Corrupted file, please check your game's Kamek files");

    // Check version number
    if (header->version != DEFAULT_VERSION)
        Error(funcs, "FATAL ERROR: Incompatible file version, please upgrade your Kamek Loader");

    // Read header data
    u32 codeSize = header->codeSize;
    u32 bssSize = header->bssSize;
    u32 ctorStart = header->ctorStart;
    u32 ctorEnd = header->ctorEnd;
    funcs->OSReport("header: bssSize=%u, codeSize=%u, ctors=%u-%u\n", bssSize, codeSize, ctorStart, ctorEnd);

    // Allocate text + bss section buffer, bail on failure
    u32 textSize = codeSize + bssSize;
    u32 text = (u32)(funcs->RKSystem->EGGSystem->alloc(textSize, 0x20));
    if (!text)
        Error(funcs, "FATAL ERROR: Out of code memory");

    // Get relocation section
    const u32* input = (u32*)(binary + sizeof(Header));
    const u32* inputEnd = (u32*)(binary + binaryLength);
    u8* output = (u8*)text;

    // Store payload start and end addresses (for debugging)
    __PayloadStart = output;
    __PayloadEnd = output + textSize;

    // Create text + bss sections
    memcpy(output, input, codeSize);
    memset(output + codeSize, 0, bssSize);
    __flush_cache(output, textSize);
    input += (codeSize / sizeof(u32));

    // Apply patches
    IGNORE_ERR(167);
    Patcher::ApplyPatches(funcs, input, inputEnd, text);
    UNIGNORE_ERR(167);

    // Synchronize to ensure caches are flushed
    __sync();
    __isync();

    // Run static initializers
    for (Func* f = (Func*)(text + ctorStart); f < (Func*)(text + ctorEnd); f++)
        (*f)();
}

void LoadFromDisc(const Functions* funcs, const char* path) {
    funcs->OSReport("{Kamek by Treeki}\nLoading Kamek binary '%s'...\n", path);

    // Locate file
    int entrynum = funcs->DVDConvertPathToEntrynum(path);
    if (entrynum < 0) {
        char err[64];
        funcs->snprintf(err, sizeof(err), "FATAL ERROR: Failed to locate file on the disc: %s", path);
        Error(funcs, err);
    }

    // Open it
    DVDFileInfo fileInfo;
    if (!funcs->DVDFastOpen(entrynum, &fileInfo))
        Error(funcs, "FATAL ERROR: Failed to open file!");

    funcs->OSReport("DVD file located: addr=%p, size=%d\n", fileInfo.startAddr, fileInfo.length);

    // Allocate read buffer
    u32 roundedLength = OSRoundUp32B(fileInfo.length);
    void* buffer = funcs->RKSystem->EGGSystem->alloc(roundedLength, -0x20);
    if (!buffer)
        Error(funcs, "FATAL ERROR: Out of file memory");

    // Read the file and close it
    funcs->DVDReadPrio(&fileInfo, buffer, roundedLength, 0, 2);
    funcs->DVDClose(&fileInfo);

    // Execute payload
    Load(funcs, (u32)buffer, fileInfo.length);

    // Free buffer and return
    funcs->RKSystem->EGGSystem->free(buffer);
    funcs->OSReport("All done!\n");
}

} // namespace Loader
