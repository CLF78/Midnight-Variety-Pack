#include <common/Common.hpp>
#include <game/system/RKSystem.hpp>
#include <game/system/SaveManager.hpp>
#include <midnight/save/SaveExpansionManager.hpp>
#include <platform/new.hpp>
#include <revolution/os/OS.h>

SaveExpansionManager SaveExpansionManager::sInstance;

void SaveExpansionManager::Read() {

    // Check if the expansion exists
    NANDFileInfo fileInfo;
    int result = NandUtil::Open(SAVEEX_FILENAME, &fileInfo, NAND_ACCESS_READ);

    // If there was an error, bail
    if (result != NandUtil::ERROR_NONE && result != NandUtil::ERROR_FILE_NOT_FOUND) {
        DEBUG_REPORT("[SAVEEX] Open returned error %d\n", result)
        mError = result;
        return;
    }

    // If the file was not found, try creating it
    if (result == NandUtil::ERROR_FILE_NOT_FOUND) {
        DEBUG_REPORT("[SAVEEX] Expansion not found! Creating...\n")
        Create();
        return;
    }

    // Get the file's length
    u32 expansionLength;
    result = NandUtil::GetLength(&fileInfo, &expansionLength);

    // If there was an error, bail
    if (result != NandUtil::ERROR_NONE) {
        DEBUG_REPORT("[SAVEEX] GetLength returned error %d\n", result)
        mError = result;
        return;
    }

    // Try to allocate a read buffer
    // Use the Kamek heap because at this point other heaps are locked
    DEBUG_REPORT("[SAVEEX] Got file length %d\n", expansionLength)
    SaveExpansion* expansion = &SaveManager::instance->expansion;
    u32 bufferLen = OSRoundUp32(expansionLength);
    u8* buffer = new (KAMEK_HEAP, 32) u8[bufferLen];

    // If it fails, bail with a generic error
    if (!buffer) {
        DEBUG_REPORT("[SAVEEX] Failed to allocate read buffer!\n")
        mError = NandUtil::ERROR_OTHER;
        return;
    }

    // Save the data
    expansion->mReadBuffer = buffer;
    expansion->mReadBufferSize = bufferLen;

    // Try reading the file
    // If it fails, bail
    result = NandUtil::Read(&fileInfo, buffer, bufferLen, 0);
    if (result != NandUtil::ERROR_NONE) {
        DEBUG_REPORT("[SAVEEX] Read returned error %d\n", result)
        mError = result;
        return;
    }

    // Close the file
    // If it fails, bail
    result = NandUtil::Close(&fileInfo);
    if (result != NandUtil::ERROR_NONE) {
        DEBUG_REPORT("[SAVEEX] Close returned error %d\n", result)
        mError = result;
        return;
    }

    // Read the data into the expansion
    // If the read fails due to corrupted/invalid data, throw an error
    if (!expansion->Read()) {
        DEBUG_REPORT("[SAVEEX] Failed to read save due to corrupted data!\n")
        mError = NandUtil::ERROR_FILE_CORRUPT;
    } else {
        DEBUG_REPORT("[SAVEEX] Read completed!\n")
        mError = NandUtil::ERROR_NONE;
    }

    // Delete the read buffer
    expansion->DeleteReadBuffer();
}

void SaveExpansionManager::Create() {

    // Get the expansion
    SaveExpansion* expansion = &SaveManager::instance->expansion;

    // Check that the file can be created, if not bail
    u32 answer;
    int result = NandUtil::Check(NANDBytesToBlocks(expansion->mWriteBufferSize), 1, &answer);

    // If the procedure failed, bail
    if (result != NandUtil::ERROR_NONE) {
        DEBUG_REPORT("[SAVEEX] Check returned error %d\n", result)
        mError = result;
        return;
    }

    // If there's not enough space, bail
    if (answer != 0) {
        DEBUG_REPORT("[SAVEEX] Cannot fit save in NAND! (%x)\n", answer)
        mError = NandUtil::ERROR_SPACE;
        return;
    }

    // Try creating the file
    result = NandUtil::Create(SAVEEX_FILENAME, NAND_PERM_OTHER_READ |
                                               NAND_PERM_GROUP_READ |
                                               NAND_PERM_OWNER_RW);

    // If the procedure failed, bail
    if (result != NandUtil::ERROR_NONE) {
        DEBUG_REPORT("[SAVEEX] Create returned error %d\n", result)
        mError = result;
        return;
    }

    // If creation was successful, try writing the file
    DEBUG_REPORT("[SAVEEX] Creation successful! Flushing to NAND...\n")
    Write();
}

void SaveExpansionManager::Write() {

    // Check if the expansion exists
    NANDFileInfo fileInfo;
    int result = NandUtil::Open(SAVEEX_FILENAME, &fileInfo, NAND_ACCESS_WRITE);

    // If there was an error, bail
    if (result != NandUtil::ERROR_NONE) {
        DEBUG_REPORT("[SAVEEX] Open returned error %d\n", result)
        mError = result;
        return;
    }

    // Try writing the file
    // If it fails, bail
    SaveExpansion* expansion = &SaveManager::instance->expansion;
    result = NandUtil::Write(&fileInfo, expansion->mWriteBuffer, expansion->mWriteBufferSize, 0);
    if (result != NandUtil::ERROR_NONE) {
        DEBUG_REPORT("[SAVEEX] Write returned error %d\n", result)
        mError = result;
        return;
    }

    // Close the file
    // If it fails, bail
    result = NandUtil::Close(&fileInfo);
    if (result != NandUtil::ERROR_NONE) {
        DEBUG_REPORT("[SAVEEX] Close returned error %d\n", result)
        mError = result;
        return;
    }
}
