#include "SaveExpansion.hpp"
#include <game/util/NandUtil.hpp>
#include <platform/string.h>
#include <revolution/os/OS.h>
#include <revolutionex/net/NETDigest.h>

bool SaveExpansion::Header::IsValid(u32 fileSize) const {

    // Check magic
    if (magic != SAVEEX_MAGIC)
        return false;

    // Check version number
    if (revision > SAVEEX_VERSION_NUMBER)
        return false;

    // Ensure the file at least has a full header
    if (headerSize > fileSize)
        return false;

    // Ensure the header contains one license offset per license
    if (offsetof(SaveExpansion::Header, licenseOffsets) + licenseCount * sizeof(u32) != headerSize)
        return false;

    // Ensure each license offset is in the file
    const u32* licenseOffsData = &licenseOffsets[0];
    for (int i = 0; i < licenseCount; i++) {
        if (licenseOffsData[i] > fileSize)
            return false;
    }

    // All checks passed!
    return true;
}

SaveExpansion::SaveExpansion() : mWriteBufferSize(GetRequiredSpace()) {

    // Initialize the save with the default values
    Init();

    // Create the write buffer since we know the write size
    mWriteBuffer = new (32) u8[mWriteBufferSize];
    memset(mWriteBuffer, 0, mWriteBufferSize);
    Write();
}

u32 SaveExpansion::GetRequiredSpace() const {

    // Get the base header size
    u32 requiredSpace = offsetof(SaveExpansion::Header, licenseOffsets);

    // Add one offset per license
    requiredSpace += sizeof(u32) * ARRAY_SIZE(mLicenses);

    // Add the space required by each license (they are all identical)
    requiredSpace += mLicenses[0].GetRequiredSpace() * ARRAY_SIZE(mLicenses);

    // Ensure the file is 32-byte aligned
    return OSRoundUp32(requiredSpace);
}

void SaveExpansion::Init() {
    for (int i = 0; i < ARRAY_SIZE(mLicenses); i++) {
        mLicenses[i].Init();
    }
}

bool SaveExpansion::Read(u8* buffer, u32 bufferSize) {

    // If buffer is null, bail
    if (!buffer)
        return true;

    // If the header is not valid, bail
    Header* header = (Header*)buffer;
    if (!header->IsValid(bufferSize))
        return false;

    // Ensure the checksum matches
    const u32 checksum = header->checksum;
    header->checksum = 0;
    const u32 checksumRecalc = NETCalcCRC32(buffer, bufferSize);
    if (checksum != checksumRecalc)
        return false;

    // Parse each license
    for (int i = 0; i < header->licenseCount && i < ARRAY_SIZE(mLicenses); i++) {
        u8* license = buffer + header->headerSize + header->licenseOffsets[i];
        const u32 licenseSize = (i == ARRAY_SIZE(mLicenses) - 1 ) ? bufferSize - header->licenseOffsets[i]
                                                            : header->licenseOffsets[i+1] - header->licenseOffsets[i];

        // If one of the license headers is invalid, bail
        if (!mLicenses[i].Read(license, licenseSize))
            return false;
    }

    return true;
}

void SaveExpansion::Write() {

    // Write the header
    Header* header = (Header*)mWriteBuffer;
    header->magic = SAVEEX_MAGIC;
    header->revision = SAVEEX_VERSION_NUMBER;
    header->headerSize = offsetof(Header, licenseOffsets) + sizeof(u32) * ARRAY_SIZE(mLicenses);
    header->checksum = 0;
    header->licenseCount = ARRAY_SIZE(mLicenses);

    // Get the space per license
    const u32 spacePerLicense = mLicenses[0].GetRequiredSpace();

    // Write each license
    u8* saveStart = (u8*)header + header->headerSize;
    u8* currLicensePtr = saveStart;
    for (int i = 0; i < ARRAY_SIZE(mLicenses); i++) {

        // Write the license and its offset
        mLicenses[i].Write(currLicensePtr);
        header->licenseOffsets[i] = currLicensePtr - saveStart;
        currLicensePtr += spacePerLicense;
    }

    // Calculate the checksum
    header->checksum = NETCalcCRC32(mWriteBuffer, mWriteBufferSize);
}
