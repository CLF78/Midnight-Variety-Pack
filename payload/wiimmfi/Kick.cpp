#include <common/Common.hpp>
#include <dwc/dwc_main.h>
#include <nw4r/ut/Lock.hpp>
#include <wiimmfi/Kick.hpp>

namespace Wiimmfi {
namespace Kick {

u32 aidsToBeKicked;

void ScheduleForAID(int aid) {
    aidsToBeKicked |= (1 << aid);
}

void ScheduleForEveryone() {
    aidsToBeKicked = 0xFFFFFFFF;
}

void CalcKick() {

    // If the aid bitfield is empty, bail
    if (!aidsToBeKicked)
        return;

    // Lock interrupts
    nw4r::ut::AutoInterruptLock lock;

    // If the bitfield is full, close all connections immediately
    if (aidsToBeKicked == 0xFFFFFFFF)
        DWC_CloseAllConnectionsHard();

    // Otherwise kick each aid separately
    else {
        for (int i = 0; i < 12; i++) {
            if (aidsToBeKicked >> i & 1)
                DWC_CloseConnectionHard(i);
        }
    }

    // Reset the bitfield
    aidsToBeKicked = 0;
}

} // namespace Kick
} // namespace Wiimmfi
