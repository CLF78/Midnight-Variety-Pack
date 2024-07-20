#include <common/Common.hpp>
#include <mvp/save/SaveExpansionSection.hpp>
#include <mvp/save/SaveExpansionCup.hpp>
#include <mvp/save/SaveExpansionRating.hpp>

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
