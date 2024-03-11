#include <common/Common.hpp>
#include <midnight/save/SaveExpansionLicense.hpp>

bool SaveExpansionLicense::Header::IsValid(u32 licenseSize) {

    // Check magic
    if (magic != SAVEEX_LICENSE_MAGIC)
        return false;

    // Ensure the license header contains one section offset per section
    if (offsetof(SaveExpansionLicense::Header, sectionOffsets) + sectionCount * sizeof(u32) != headerSize)
        return false;

    // Ensure each section offset is in the license
    u32* sectionOffsData = &sectionOffsets[0];
    for (int i = 0; i < sectionCount; i++) {
        if (sectionOffsData[i] > licenseSize)
            return false;
    }

    // All checks passed!
    return true;
}

SaveExpansionLicense::SaveExpansionLicense() {

    // Construct each section
    for (int i = 0; i < ARRAY_SIZE(mSections); i++) {
        mSections[i] = SaveExpansionSection::CreateByID(i);
    }
}

u32 SaveExpansionLicense::GetRequiredSpace() {

    // Get the base header size
    u32 requiredSpace = offsetof(SaveExpansionLicense::Header, sectionOffsets);

    // Add one offset per section
    requiredSpace += sizeof(u32) * ARRAY_SIZE(mSections);

    // Get the required space of each section, plus the magic
    for (int i = 0; i < ARRAY_SIZE(mSections); i++) {
        requiredSpace += mSections[i]->GetRequiredSpace();
        requiredSpace += sizeof(u32);
    }

    return requiredSpace;
}

void SaveExpansionLicense::Init() {
    for (int i = 0; i < ARRAY_SIZE(mSections); i++) {
        mSections[i]->Init();
    }
}

void SaveExpansionLicense::Read(u8* buffer, u32 bufferSize) {

    // If the header for the license is not valid, bail
    Header* header = (Header*)buffer;
    if (!header->IsValid(bufferSize))
        return;

    // Parse each section
    for (int i = 0; i < header->sectionCount; i++) {

        // Get the section pointer
        u8* section = buffer + header->headerSize + header->sectionOffsets[i];

        // Determine which section it is and parse the data using its virtual function
        for (int j = 0; j < ARRAY_SIZE(mSections); j++) {
            SaveExpansionSection* section = mSections[j];
            if (*(u32*)buffer == section->GetMagic()) {
                section->Read(buffer + sizeof(u32));
                break;
            }
        }
    }
}

void SaveExpansionLicense::Write(u8* buffer) {

    // Write the header
    Header* header = (Header*)buffer;
    header->magic = SAVEEX_LICENSE_MAGIC;
    header->headerSize = offsetof(Header, sectionOffsets) + sizeof(u32) * ARRAY_SIZE(mSections);
    header->sectionCount = ARRAY_SIZE(mSections);

    // Write each section
    u8* licenseStart = (u8*)header + header->headerSize;
    u8* currLicensePtr = licenseStart;
    for (int i = 0; i < header->sectionCount; i++) {

        // Get section
        SaveExpansionSection* section = mSections[i];

        // Write magic
        *(u32*)currLicensePtr++ = section->GetMagic();

        // Write the section and its offset
        section->Write(currLicensePtr);
        header->sectionOffsets[i] = currLicensePtr - licenseStart;

        // Go to the next section
        currLicensePtr += section->GetRequiredSpace();
    }
}
