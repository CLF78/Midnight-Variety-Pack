#include <common/Common.hpp>
#include <dwc/dwc_match.h>
#include <gs/common/gsAvailable.h>
#include <gs/common/gsPlatformUtil.h>
#include <gs/common/gsSocketRevolution.h>
#include <gs/natneg/natify.h>
#include <gs/natneg/natneg.h>
#include <revolution/os/OS.h>

namespace Wiimmfi {
namespace Natify {

bool sDoingNatify;

void Callback(BOOL success, NAT* theNat) {
    sDoingNatify = false;
    // OSReport("[LE-CODE]: NATify done for pid %d\n", stpMatchCnt->profileId);
}

void Try() {
    sDoingNatify = true;
    NNStartNatDetection(Callback);
}

void Calc() {
    if (sDoingNatify)
        NegotiateThink(nullptr);
}

} // namespace Natify
} // namespace Wiimmfi
