#include <common/Common.hpp>
#include <midnight/save/SaveExpansion.hpp>
#include <revolution/os/OS.h>
#include <revolutionex/net/NETDigest.h>

bool SaveExpansion::Header::IsValid(u32 fileSize) {

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
    u32* licenseOffsData = &licenseOffsets[0];
    for (int i = 0; i < licenseCount; i++) {
        if (licenseOffsData[i] > fileSize)
            return false;
    }

    // All checks passed!
    return true;
}

u32 SaveExpansion::GetRequiredSpace() {

    // Get the base header size
    u32 requiredSpace = offsetof(SaveExpansion::Header, licenseOffsets);

    // Add one offset per license
    requiredSpace += sizeof(u32) * ARRAY_SIZE(mLicenses);

    // Add the space required by each license (they are all identical)
    requiredSpace += mLicenses[0].GetRequiredSpace() * ARRAY_SIZE(mLicenses);

    // Ensure the file is 32-byte aligned
    return OSRoundUp32B(requiredSpace);
}

void SaveExpansion::Init() {
    for (int i = 0; i < ARRAY_SIZE(mLicenses); i++) {
        mLicenses[i].Init();
    }
}

bool SaveExpansion::Read(u8* buffer, u32 bufferSize) {

    // If buffer is null, bail
    if (!buffer)
        return false;

    // If the header is not valid, bail
    Header* header = (Header*)buffer;
    if (!header->IsValid(bufferSize))
        return false;

    // Ensure the checksum matches
    u32 checksum = header->checksum;
    header->checksum = 0;
    u32 checksumRecalc = NETCalcCRC32(buffer, bufferSize);
    if (checksum != checksumRecalc)
        return false;

    // Parse each license
    for (int i = 0; i < header->licenseCount && i < ARRAY_SIZE(mLicenses); i++) {
        u8* license = buffer + header->headerSize + header->licenseOffsets[i];
        u32 licenseSize = (i == ARRAY_SIZE(mLicenses) - 1 ) ? bufferSize - header->licenseOffsets[i]
                                                            : header->licenseOffsets[i+1] - header->licenseOffsets[i];
        mLicenses[i].Read(license, licenseSize);
    }

    return true;
}

bool SaveExpansion::Write(u8* buffer, u32 bufferSize) {

    // If buffer is null, bail
    if (!buffer)
        return false;

    // Write the header
    Header* header = (Header*)buffer;
    header->magic = SAVEEX_MAGIC;
    header->revision = SAVEEX_VERSION_NUMBER;
    header->headerSize = offsetof(Header, licenseOffsets) + sizeof(u32) * ARRAY_SIZE(mLicenses);
    header->checksum = 0;
    header->licenseCount = ARRAY_SIZE(mLicenses);

    // Get the space per license
    u32 spacePerLicense = mLicenses[0].GetRequiredSpace();

    // Write each license
    u8* saveStart = (u8*)header + header->headerSize;
    u8* currLicensePtr = saveStart;
    for (int i = 0; i < header->licenseCount; i++) {

        // Write the license and its offset
        mLicenses[i].Write(currLicensePtr);
        header->licenseOffsets[i] = currLicensePtr - saveStart;
        currLicensePtr += spacePerLicense;
    }

    // Calculate the checksum
    header->checksum = NETCalcCRC32(buffer, bufferSize);
    return true;
}
