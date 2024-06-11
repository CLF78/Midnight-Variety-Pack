#include <common/Common.hpp>
#include <game/system/RKSystem.hpp>
#include <game/system/SaveManager.hpp>
#include <midnight/save/SaveExpansionManager.hpp>
#include <midnight/util/NANDFile.hpp>
#include <revolution/os/OS.h>

namespace SaveExpansionManager {

int sError = NandUtil::ERROR_NONE;
int sCheckError = NandUtil::CHECK_ERROR_NONE;

int Read() {

    // Try to open the file
    NANDFileHandler fileHandler(SAVEEX_FILENAME, NAND_ACCESS_READ);

    // If there was an error, bail
    int result = fileHandler.mResult;
    if (result != NandUtil::ERROR_NONE && result != NandUtil::ERROR_FILE_NOT_FOUND) {
        LOG_DEBUG("[SAVEEX] Open returned error %d\n", result);
        return result;
    }

    // If the file was not found, try creating it
    if (result == NandUtil::ERROR_FILE_NOT_FOUND) {
        LOG_DEBUG("[SAVEEX] Expansion not found! Creating...\n");
        return Write();
    }

    // Get the file's length
    u32 expansionLength;
    result = NandUtil::GetLength(&fileHandler.mInfo, &expansionLength);

    // If there was an error, close the file and bail
    if (result != NandUtil::ERROR_NONE) {
        LOG_DEBUG("[SAVEEX] GetLength returned error %d\n", result);
        return result;
    }

    // Try to allocate a read buffer
    LOG_DEBUG("[SAVEEX] Got file length %d\n", expansionLength);
    u32 bufferLen = OSRoundUp32(expansionLength);
    NANDFileBuffer buffer(bufferLen, KAMEK_HEAP);

    // If it fails, bail with a generic error
    if (!buffer.mBuffer) {
        LOG_DEBUG("[SAVEEX] Failed to allocate read buffer!\n");
        return NandUtil::ERROR_OTHER;
    }

    // Try reading the file
    // If it fails, bail
    result = NandUtil::Read(&fileHandler.mInfo, buffer.mBuffer, bufferLen, 0);
    if (result != NandUtil::ERROR_NONE) {
        LOG_DEBUG("[SAVEEX] Read returned error %d\n", result);
        return result;
    }

    // Read the data into the expansion
    // If the read fails due to corrupted/invalid data, return an error
    if (!SaveManager::instance->expansion.Read(buffer.mBuffer, bufferLen)) {
        LOG_DEBUG("[SAVEEX] Failed to read save due to corrupted data!\n");
        return NandUtil::ERROR_FILE_CORRUPT;
    }

    // Read completed!
    LOG_DEBUG("[SAVEEX] Read completed!\n");
    return NandUtil::ERROR_NONE;
}

int SaveExpansionManager::Create() {

    // Reset the check error
    sCheckError = NandUtil::CHECK_ERROR_NONE;

    // Check that the file can be created, if not bail
    u32 answer;
    int result = NandUtil::Check(NANDBytesToBlocks(SaveManager::instance->expansion.mWriteBufferSize), 1, &answer);

    // If the procedure failed, bail
    if (result != NandUtil::ERROR_NONE) {
        LOG_DEBUG("[SAVEEX] Check returned error %d\n", result);
        sCheckError = NandUtil::CHECK_ERROR_FAIL;
        return result;
    }

    // If there's not enough space, bail
    if (answer != 0) {
        LOG_DEBUG("[SAVEEX] Cannot fit save in NAND! (%x)\n", answer);

        if (answer & (NAND_CHECK_HOME_NO_SPACE | NAND_CHECK_SYS_NO_SPACE))
            sCheckError |= NandUtil::CHECK_ERROR_BLOCKS;

        if (answer & (NAND_CHECK_HOME_NO_INODES | NAND_CHECK_SYS_NO_INODES))
            sCheckError |= NandUtil::CHECK_ERROR_INODES;

        return NandUtil::ERROR_SPACE;
    }

    // Try creating the file
    result = NandUtil::Create(SAVEEX_FILENAME, NAND_PERM_OTHER_READ |
                                               NAND_PERM_GROUP_READ |
                                               NAND_PERM_OWNER_RW);

    // Return the error code
    if (result != NandUtil::ERROR_NONE)
        LOG_DEBUG("[SAVEEX] Create returned error %d\n", result);
    else
        LOG_DEBUG("[SAVEEX] Creation successful!\n");

    return result;
}

int SaveExpansionManager::Write() {

    // Check if the file exists
    u32 fileType;
    int result = NandUtil::GetType(SAVEEX_FILENAME, &fileType);

    // If there was an error, bail
    if (result != NandUtil::ERROR_NONE) {
        LOG_DEBUG("[SAVEEX] GetType returned %d\n", result);
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
        LOG_DEBUG("[SAVEEX] Open returned error %d\n", result);
        return result;
    }

    // Try writing the file
    // If it fails, bail
    SaveExpansion* expansion = &SaveManager::instance->expansion;
    result = NandUtil::Write(&fileHandler.mInfo, expansion->mWriteBuffer, expansion->mWriteBufferSize, 0);
    if (result != NandUtil::ERROR_NONE) {
        LOG_DEBUG("[SAVEEX] Write returned error %d\n", result);
        return result;
    }

    // Return the result
    LOG_DEBUG("[SAVEEX] Write successful!\n");
    return result;
}

int Delete() {

    // NANDUtil already deals with the file not existing, so we are relatively safe
    LOG_DEBUG("[SAVEEX] Deleting save expansion data...\n");
    return NandUtil::Delete(SAVEEX_FILENAME);
}

} // namespace SaveExpansionManager
