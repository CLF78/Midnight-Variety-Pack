#include "SaveExpansionSection.hpp"
#include "SaveExpansionCup.hpp"
#include "SaveExpansionRating.hpp"

SaveExpansionSection* SaveExpansionSection::CreateByID(u32 id) {
    switch (id) {
        case SECTION_CUP_COMPLETION:
            return new SaveExpansionCup();

        case SECTION_RATING:
            return new SaveExpansionRating();

        default:
            return nullptr;
    }
}
