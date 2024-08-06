#pragma once
#include "SaveExpansionSection.hpp"

class SaveExpansionLicense {
public:

    struct Header {
        bool IsValid(u32 licenseSize) const;

        u32 magic;
        u32 headerSize;
        u32 sectionCount;
        u32 sectionOffsets[]; // from end of header
    };

    SaveExpansionLicense();

    u32 GetRequiredSpace() const;
    SaveExpansionSection* GetSection(u32 id) const { return mSections[id]; }

    void Init();
    bool Read(u8* buffer, u32 bufferSize);
    void Write(u8* buffer);

    SaveExpansionSection* mSections[SaveExpansionSection::SECTION_COUNT];
};
