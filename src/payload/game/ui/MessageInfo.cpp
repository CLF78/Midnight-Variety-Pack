#include <common/Common.hpp>

//////////////////////////////
// Conditional Text Display //
//////////////////////////////

// MessageInfo::MessageInfo() patch
// Initialize custom fields to zero
kmWrite8(0x805CDA2C, 0x90);
