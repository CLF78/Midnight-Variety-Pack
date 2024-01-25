#include <common/Common.hpp>
#include <game/kart/KartCollide.hpp>
#include <game/system/CourseMap.hpp>
#include <game/system/RaceManager.hpp>

///////////////////////////
// Custom Track Features //
///////////////////////////

// Reimplement riidefi's Conditional Out Of Bounds feature
kmBranchDefCpp(0x80571858, 0x805718B4, void, KartObjectProxy* kobj) {

    // Get collision from the pointers (this lets us avoid glue code)
    KartCollide* coll = kobj->pointers->kartCollide;

    // Get the closest respawn area
    s16 areaId = CourseMap::instance->getClosestAreaByType(kobj->getPlayerPosition(),
                                                           coll->respawnArea,
                                                           MapdataAreaBase::FALL_BOUNDARY);

    // If none were found, bail
    if (areaId == -1)
        return;

    // Get the area settings
    u16 p1 = CourseMap::instance->mpArea->entries[areaId]->mpData->parameters[0];
    u16 p2 = CourseMap::instance->mpArea->entries[areaId]->mpData->parameters[1];

    // Get the player's checkpoint
    u16 ckpt = RaceManager::instance->players[kobj->getPlayerIdx()]->checkpointId;

    // Whitelist mode: do not trigger a fall boundary if we are not in p1 <= ckpt < p2
    if (p1 < p2 && (ckpt < p1 || ckpt >= p2))
        return;

    // Blacklist mode: trigger a fall boundary if we are not in p2 <= ckpt < p1
    if (p1 > p2 && (ckpt >= p2 && ckpt < p1))
        return;

    // Activate out of bounds
    coll->respawnArea = areaId;
    kobj->setOobCamera();
    coll->activateOob(false, nullptr, false, false);
}
