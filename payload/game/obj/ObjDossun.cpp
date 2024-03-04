#include <common/Common.hpp>
#include <game/gfx/AnmHolder.hpp>
#include <game/gfx/ModelTransformator.hpp>

////////////////////
// Game Bug Fixes //
////////////////////

// Prevent Thwomps from crashing the game when they are hit before they land the first time
// Credits: MrBean35000vr
kmBranchDefCpp(0x80760A84, 0x80760A98, float, ModelTransformator* transformator, int idx) {

	// Get animation frame count, otherwise a dummy value
	AnmHolder* anm = transformator->GetAnmHolder(idx);
	return (anm != nullptr) ? anm->GetFrameCount() : 0.0f;
}
