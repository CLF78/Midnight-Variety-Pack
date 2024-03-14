#include <common/Common.hpp>
#include <game/system/SaveManager.hpp>

///////////////////////////////
// Savegame Expansion System //
///////////////////////////////

// LicenseDeletePage::onRefocus() patch
// Delete the expansion license along the original save license
kmCallDefCpp(0x805EE3D8, void, SaveManager* self, u32 licenseId) {

    // Original call
    self->eraseLicense(licenseId);

    // Initialize expansion license
    self->expansion.mLicenses[licenseId].Init();
}
