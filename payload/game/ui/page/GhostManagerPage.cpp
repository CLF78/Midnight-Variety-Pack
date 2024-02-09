#include <common/Common.hpp>

///////////////////////////
// Custom Engine Classes //
///////////////////////////

// GhostManagerPage::setupGhostReplay() patch
// Prevent overwriting CC when starting a ghost replay
kmWrite32(0x805E1EB0, 0x60000000);

// GhostManagerPage::setupGhostRace() patch
// Prevent overwriting CC when starting a time trial against a ghost
kmWrite32(0x805E1D0C, 0x60000000);
