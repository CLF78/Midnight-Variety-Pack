#include <common/Common.hpp>
#include <platform/string.h>
#include <revolution/dvd/dvd.h>
#include <revolution/os/OS.h>
#include <revolution/os/OSCache.h>
#include <revolution/os/OSLoMem.h>
#include <Loader.hpp>
#include <Patcher.hpp>

namespace Loader {

void Load(const Functions* funcs, u32 binary, u32 binaryLength) {
    LOG_DEBUG("Parsing Kamek binary...");
    const Header* header = (const Header*)binary;

    // Check magic
    if (header->magic1 != DEFAULT_MAGIC1 || header->magic2 != DEFAULT_MAGIC2) {
        LOG_FATAL("The Kamek binary is corrupted.\nPlease check your game's Kamek files.");
    }

    // Check version number
    if (header->version != DEFAULT_VERSION) {
        LOG_FATAL("The Kamek binary version is incompatible with this loader.\n"
                  "File Version: %d - Expected Version: %d", header->version, DEFAULT_VERSION);
    }

    // Read header data
    u32 codeSize = header->codeSize;
    u32 bssSize = header->bssSize;
    u32 ctorStart = header->ctorStart;
    u32 ctorEnd = header->ctorEnd;
    u32 textSize = codeSize + bssSize;
    LOG_DEBUG("Payload Size: %#x (Code: %#x, Constructors: %#x-%#x, BSS: %#x)",
              textSize, codeSize, ctorStart, ctorEnd, bssSize);

    // Allocate text + bss section buffer, bail on failure
    LOG_DEBUG("Allocating payload buffer...");
    u32 text = (u32)(funcs->RKSystem->EGGSystem->alloc(textSize, 0x20));
    if (!text) {
        LOG_FATAL("Failed to allocate payload buffer for Kamek binary.");
    }

    // Store payload start and end addresses (for debugging)
    u8* output = (u8*)text;
    __PayloadStart = output;
    __PayloadEnd = output + textSize;
    LOG_DEBUG("Payload allocated at %#x - %#x", __PayloadStart, __PayloadEnd);

    // Get relocation section
    const u32* input = (u32*)(binary + sizeof(Header));
    const u32* inputEnd = (u32*)(binary + binaryLength);

    // Create text + bss sections
    LOG_DEBUG("Copying data...");
    memcpy(output, input, codeSize);
    memset(output + codeSize, 0, bssSize);
    __flush_cache(output, textSize);
    input += (codeSize / sizeof(u32));

    // Apply patches
    LOG_DEBUG("Relocation data at (%#x-%#x)", input, inputEnd);
    Patcher::ApplyPatches(input, inputEnd, text);

    // Synchronize to ensure caches are flushed
    __sync();
    __isync();

    // Run static initializers
    LOG_DEBUG("Running constructors...");
    for (Func* f = (Func*)(text + ctorStart); f < (Func*)(text + ctorEnd); f++)
        (*f)();
}

void LoadFromDisc(const Functions* funcs, const char* path) {
    LOG_DEBUG("Loading Kamek binary from '%s'...", path);

    // Locate file
    int entrynum = funcs->DVDConvertPathToEntrynum(path);
    if (entrynum < 0) {
        LOG_FATAL("Failed to locate Kamek binary.\nExpected path: " KAMEK_ROOT_DIR "%s", path);
    }

    // Open it
    DVDFileInfo fileInfo;
    if (!funcs->DVDFastOpen(entrynum, &fileInfo)) {
        LOG_FATAL("Failed to locate Kamek binary.\nExpected path: " KAMEK_ROOT_DIR "%s", path);
    }

    // Log file address and size
    LOG_DEBUG("File located. Start Address: %p, File Size: %#x", fileInfo.startAddr, fileInfo.length);

    // Round up the file length to 32 bytes
    u32 roundedLength = OSRoundUp32(fileInfo.length);
    LOG_DEBUG("Allocating file buffer...");

    // Allocate the buffer
    void* buffer = funcs->RKSystem->EGGSystem->alloc(roundedLength, -0x20);
    if (!buffer) {
        LOG_FATAL("Failed to allocate file buffer for Kamek binary.");
    }

    // Read the file
    LOG_DEBUG("Memory allocated successfully at %#x. Reading data...",  buffer);
    int readSize = funcs->DVDReadPrio(&fileInfo, buffer, roundedLength, 0, 2);
    if (readSize < 0) {
        LOG_FATAL("Failed to read Kamek binary into memory.\nResult code: %d", readSize);
    }

    // Close it
    funcs->DVDClose(&fileInfo);

    // Parse the binary
    Load(funcs, (u32)buffer, fileInfo.length);

    // Free buffer and return
    LOG_DEBUG("Freeing file buffer...");
    funcs->RKSystem->EGGSystem->free(buffer);
    LOG_DEBUG("Loading sequence completed.");
}

} // namespace Loader
