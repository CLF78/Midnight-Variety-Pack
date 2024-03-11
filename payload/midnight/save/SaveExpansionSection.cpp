#include <common/Common.hpp>
#include <midnight/save/SaveExpansionSection.hpp>
#include <midnight/save/SaveExpansionCup.hpp>

SaveExpansionSection* SaveExpansionSection::CreateByID(u32 id) {
    switch(id) {
        case SECTION_CUP_COMPLETION:
            return new SaveExpansionCup();

        default:
            return nullptr;
    }
}
