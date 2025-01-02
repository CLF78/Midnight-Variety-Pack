#include "SaveExpansionSection.hpp"
#include "SaveExpansionCup.hpp"
#include "SaveExpansionDrift.hpp"
#include "SaveExpansionRating.hpp"

SaveExpansionSection* SaveExpansionSection::CreateByID(u32 id) {
    switch (id) {
        case SECTION_CUP_COMPLETION:
            return new SaveExpansionCup();

        case SECTION_RATING:
            return new SaveExpansionRating();

        case SECTION_DRIFT:
            return new SaveExpansionDrift();

        default:
            return nullptr;
    }
}
