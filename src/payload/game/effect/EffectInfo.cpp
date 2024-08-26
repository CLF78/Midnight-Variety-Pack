#include "EffectInfo.hpp"
#include <mvp/race/DriftTiers.hpp>

/////////////////////////////////////
// Ultra Miniturbos Delete Effects //
/////////////////////////////////////

REPLACE void EffectInfo::clear() {
    REPLACED();
    delete (ExpPlayerEffects::umtEffects);
    ExpPlayerEffects::umtEffects = nullptr;
    delete (ExpPlayerEffects::smtEffects);
    ExpPlayerEffects::smtEffects = nullptr;
}
