#include <common/Common.hpp>

class SaveExpansionSection {
public:

    // The section types (must be added for the section to be parsed)
    enum SectionID {
        SECTION_CUP_COMPLETION,
        SECTION_COUNT,
    };

    // The base structure of a section in raw form
    struct RawData {
        u32 magic;
        u8 data[];
    };

    // Gets the magic of the section
    virtual u32 GetMagic() = 0;

    // Gets the required space to write this section to memory, excluding the magic
    virtual u32 GetRequiredSpace() = 0;

    // Initializes the section
    virtual void Init() = 0;

    // Fills the section's data from the given buffer, the magic is excluded
    virtual void Read(u8* buffer) = 0;

    // Writes the section's data to the given buffer, the magic is excluded
    virtual void Write(u8* buffer) = 0;

    // Creates a section
    static SaveExpansionSection* CreateByID(u32 id);
};
