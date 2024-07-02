#include <common/Common.hpp>
#include <midnight/save/SaveExpansionSection.hpp>
#include <midnight/save/SaveExpansionCup.hpp>
#include <midnight/save/SaveExpansionRating.hpp>

SaveExpansionSection* SaveExpansionSection::CreateByID(u32 id) {
    switch(id) {
        case SECTION_CUP_COMPLETION:
            return new SaveExpansionCup();

        case SECTION_RATING:
            return new SaveExpansionRating();

        default:
            return nullptr;
    }
}
