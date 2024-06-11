#include <common/Common.hpp>

///////////////////////////
// Custom Engine Classes //
///////////////////////////

// ObjDcPillar::ObjDcPillar() patches
// Patch CC checks to read the 150cc setting on higher CCs
kmWrite32(0x807FEDE4, 0x60000000);
kmWrite32(0x807FEF4C, 0x60000000);
