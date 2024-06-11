#include <common/Common.hpp>

/////////////////////////////////
// Online Explosion Damage Fix //
/////////////////////////////////

// KartActionRealLocal::calc() patch
// Do not replace bomb damage with spinouts
// Credits: MrBean35000vr
kmWrite16(0x80569F68, 0x4800);
