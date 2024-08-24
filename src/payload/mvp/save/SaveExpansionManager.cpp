#include "SaveExpansionManager.hpp"
#include <game/system/RKSystem.hpp>
#include <game/system/SaveManager.hpp>
#include <mvp/util/NANDFile.hpp>
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
        LOG_ERROR("Open returned error %d", result);
        return result;
    }

    // If the file was not found, try creating it
    if (result == NandUtil::ERROR_FILE_NOT_FOUND) {
        LOG_DEBUG("Expansion not found! Creating...");
        return Write();
    }

    // Get the file's length
    u32 expansionLength = 0;
    result = NandUtil::GetLength(&fileHandler.mInfo, &expansionLength);

    // If there was an error, close the file and bail
    if (result != NandUtil::ERROR_NONE) {
        LOG_ERROR("GetLength returned error %d", result);
        return result;
    }

    // Try to allocate a read buffer
    LOG_DEBUG("File length is %d bytes.", expansionLength);
    const u32 bufferLen = OSRoundUp32(expansionLength);
    const NANDFileBuffer buffer(bufferLen, KAMEK_HEAP);

    // If it fails, bail with a generic error
    if (!buffer.mBuffer) {
        LOG_ERROR("Failed to allocate read buffer!");
        return NandUtil::ERROR_OTHER;
    }

    // Try reading the file
    // If it fails, bail
    result = NandUtil::Read(&fileHandler.mInfo, buffer.mBuffer, bufferLen, 0);
    if (result != NandUtil::ERROR_NONE) {
        LOG_ERROR("Read returned error %d", result);
        return result;
    }

    // Read the data into the expansion
    // If the read fails due to corrupted/invalid data, return an error
    if (!SaveManager::instance->expansion.Read(buffer.mBuffer, bufferLen)) {
        LOG_ERROR("Failed to read save due to corrupted data!");
        return NandUtil::ERROR_FILE_CORRUPT;
    }

    // Read completed!
    LOG_DEBUG("Read completed!");
    return NandUtil::ERROR_NONE;
}

int Create() {

    // Reset the check error
    sCheckError = NandUtil::CHECK_ERROR_NONE;

    // Check that the file can be created, if not bail
    u32 answer = 0;
    int result = NandUtil::Check(NANDBytesToBlocks(SaveManager::instance->expansion.mWriteBufferSize), 1,
                                 &answer);

    // If the procedure failed, bail
    if (result != NandUtil::ERROR_NONE) {
        LOG_ERROR("Check returned error %d", result);
        sCheckError = NandUtil::CHECK_ERROR_FAIL;
        return result;
    }

    // If there's not enough space, bail
    if (answer != 0) {
        LOG_ERROR("Cannot fit save in NAND! Error Code: %x", answer);

        if (answer & (NAND_CHECK_HOME_NO_SPACE | NAND_CHECK_SYS_NO_SPACE)) {
            sCheckError |= NandUtil::CHECK_ERROR_BLOCKS;
        }

        if (answer & (NAND_CHECK_HOME_NO_INODES | NAND_CHECK_SYS_NO_INODES)) {
            sCheckError |= NandUtil::CHECK_ERROR_INODES;
        }

        return NandUtil::ERROR_SPACE;
    }

    // Try creating the file
    result = NandUtil::Create(SAVEEX_FILENAME,
                              NAND_PERM_OTHER_READ | NAND_PERM_GROUP_READ | NAND_PERM_OWNER_RW);

    // Return the error code
    if (result != NandUtil::ERROR_NONE) {
        LOG_ERROR("Create returned error %d", result);
    }
    else {
        LOG_DEBUG("Creation successful!");
    }

    return result;
}

int Write() {

    // Check if the file exists
    u32 fileType = NandUtil::TYPE_NONE;
    int result = NandUtil::GetType(SAVEEX_FILENAME, &fileType);

    // If there was an error, bail
    if (result != NandUtil::ERROR_NONE) {
        LOG_ERROR("GetType returned %d", result);
        return result;
    }

    // If the file was not found, try creating it
    // If it fails, bail
    if (fileType == NandUtil::TYPE_NONE) {
        result = Create();
        if (result != NandUtil::ERROR_NONE) {
            return result;
        }
    }

    // Try opening the new file
    // If it fails, bail
    NANDFileHandler fileHandler(SAVEEX_FILENAME, NAND_ACCESS_WRITE);
    result = fileHandler.mResult;
    if (result != NandUtil::ERROR_NONE) {
        LOG_ERROR("Open returned error %d", result);
        return result;
    }

    // Try writing the file
    // If it fails, bail
    SaveExpansion* expansion = &SaveManager::instance->expansion;
    result = NandUtil::Write(&fileHandler.mInfo, expansion->mWriteBuffer, expansion->mWriteBufferSize, 0);
    if (result != NandUtil::ERROR_NONE) {
        LOG_ERROR("Write returned error %d", result);
        return result;
    }

    // Return the result
    LOG_DEBUG("Write successful!");
    return result;
}

int Delete() {

    // NandUtil already deals with the file not existing, so we are relatively safe
    LOG_DEBUG("Deleting save expansion data...");
    return NandUtil::Delete(SAVEEX_FILENAME);
}

} // namespace SaveExpansionManager
