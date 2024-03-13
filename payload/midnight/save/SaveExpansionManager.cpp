#include <common/Common.hpp>
#include <game/system/RKSystem.hpp>
#include <game/system/SaveManager.hpp>
#include <midnight/save/SaveExpansionManager.hpp>
#include <midnight/util/NANDFile.hpp>
#include <platform/new.hpp>
#include <revolution/os/OS.h>

namespace SaveExpansionManager {

int sError = NandUtil::ERROR_NONE;

int Read() {

    // Try to open the file
    NANDFileHandler fileHandler(SAVEEX_FILENAME, NAND_ACCESS_READ);

    // If there was an error, bail
    int result = fileHandler.mResult;
    if (result != NandUtil::ERROR_NONE && result != NandUtil::ERROR_FILE_NOT_FOUND) {
        DEBUG_REPORT("[SAVEEX] Open returned error %d\n", result)
        return result;
    }

    // If the file was not found, try creating it
    if (result == NandUtil::ERROR_FILE_NOT_FOUND) {
        DEBUG_REPORT("[SAVEEX] Expansion not found! Creating...\n")
        return Write();
    }

    // Get the file's length
    u32 expansionLength;
    result = NandUtil::GetLength(&fileHandler.mInfo, &expansionLength);

    // If there was an error, close the file and bail
    if (result != NandUtil::ERROR_NONE) {
        DEBUG_REPORT("[SAVEEX] GetLength returned error %d\n", result)
        return result;
    }

    // Try to allocate a read buffer
    DEBUG_REPORT("[SAVEEX] Got file length %d\n", expansionLength)
    u32 bufferLen = OSRoundUp32(expansionLength);
    NANDFileBuffer buffer(bufferLen, KAMEK_HEAP);

    // If it fails, bail with a generic error
    if (!buffer.mBuffer) {
        DEBUG_REPORT("[SAVEEX] Failed to allocate read buffer!\n")
        return NandUtil::ERROR_OTHER;
    }

    // Try reading the file
    // If it fails, bail
    result = NandUtil::Read(&fileHandler.mInfo, buffer.mBuffer, bufferLen, 0);
    if (result != NandUtil::ERROR_NONE) {
        DEBUG_REPORT("[SAVEEX] Read returned error %d\n", result)
        return result;
    }

    // Read the data into the expansion
    // If the read fails due to corrupted/invalid data, return an error
    if (!SaveManager::instance->expansion.Read(buffer.mBuffer, bufferLen)) {
        DEBUG_REPORT("[SAVEEX] Failed to read save due to corrupted data!\n")
        return NandUtil::ERROR_FILE_CORRUPT;
    }

    // Read completed!
    DEBUG_REPORT("[SAVEEX] Read completed!\n")
    return NandUtil::ERROR_NONE;
}

int SaveExpansionManager::Create() {

    // Check that the file can be created, if not bail
    u32 answer;
    int result = NandUtil::Check(NANDBytesToBlocks(SaveManager::instance->expansion.mWriteBufferSize), 1, &answer);

    // If the procedure failed, bail
    if (result != NandUtil::ERROR_NONE) {
        DEBUG_REPORT("[SAVEEX] Check returned error %d\n", result)
        return result;
    }

    // If there's not enough space, bail
    if (answer != 0) {
        DEBUG_REPORT("[SAVEEX] Cannot fit save in NAND! (%x)\n", answer)
        return NandUtil::ERROR_SPACE;
    }

    // Try creating the file
    result = NandUtil::Create(SAVEEX_FILENAME, NAND_PERM_OTHER_READ |
                                               NAND_PERM_GROUP_READ |
                                               NAND_PERM_OWNER_RW);

    // Return the error code
    if (result != NandUtil::ERROR_NONE)
        DEBUG_REPORT("[SAVEEX] Create returned error %d\n", result)
    else
        DEBUG_REPORT("[SAVEEX] Creation successful!\n")

    return result;
}

int SaveExpansionManager::Write() {

    // Check if the file exists
    u32 fileType;
    int result = NandUtil::GetType(SAVEEX_FILENAME, &fileType);

    // If there was an error, bail
    if (result != NandUtil::ERROR_NONE) {
        DEBUG_REPORT("[SAVEEX] GetType returned %d\n", result)
        return result;
    }

    // If the file was not found, try creating it
    // If it fails, bail
    if (fileType == NandUtil::TYPE_NONE) {
        result = Create();
        if (result != NandUtil::ERROR_NONE)
            return result;
    }

    // Try opening the new file
    // If it fails, bail
    NANDFileHandler fileHandler(SAVEEX_FILENAME, NAND_ACCESS_WRITE);
    result = fileHandler.mResult;
    if (result != NandUtil::ERROR_NONE) {
        DEBUG_REPORT("[SAVEEX] Open returned error %d\n", result)
        return result;
    }

    // Try writing the file
    // If it fails, bail
    SaveExpansion* expansion = &SaveManager::instance->expansion;
    result = NandUtil::Write(&fileHandler.mInfo, expansion->mWriteBuffer, expansion->mWriteBufferSize, 0);
    if (result != NandUtil::ERROR_NONE) {
        DEBUG_REPORT("[SAVEEX] Write returned error %d\n", result)
        return result;
    }

    // Return the result
    DEBUG_REPORT("[SAVEEX] Write successful!\n")
    return result;
}

int Delete() {

    // NANDUtil already deals with the file not existing, so we are relatively safe
    DEBUG_REPORT("[SAVEEX] Deleting save expansion data...\n")
    return NandUtil::Delete(SAVEEX_FILENAME);
}

} // namespace SaveExpansionManager
