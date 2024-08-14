#include "LakituControllerPlayer.hpp"
#include <game/system/RaceManager.hpp>

//////////////////
// Lap Modifier //
//////////////////

// Set the correct animation frame for the current lap
kmHookFn void SetLapBoardFrame(LakituControllerPlayer* self) {

    // Get player id and their max lap
    const u8 playerIdx = self->mpProxy->getPlayerIdx();
    const u8 lap = RaceManager::instance->players[playerIdx]->maxLap;

    // Get the frame and set it
    const float frame = (lap < 2) ? 0.0f : lap - 2;
    AnmHolder* lapAnim = self->lapModel->modelTransformator->GetAnmHolder(3);
    lapAnim->UpdateRateAndSetFrame(frame);
}

// Glue code
kmBranchDefAsm(0x80723D04, 0x80723D74) {
    nofralloc

    // Call C++ code
    mr r3, r30
    bl SetLapBoardFrame
    blr
}
