#include <game/ui/input/MultiControlInputManager.hpp>

///////////////////
// Online Voting //
///////////////////

// Allow wrapping the button selection on the Vote/Random and Repick prompt
kmWrite16(0x8065296A, MultiControlInputManager::Y_WRAP);
