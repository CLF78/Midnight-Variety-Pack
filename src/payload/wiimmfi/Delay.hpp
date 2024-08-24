#pragma once

namespace Wiimmfi {
namespace Delay {

// Calculates the delay
void Calc(u32 frameCount);

// Applies the delay to the given timer
u32 Apply(u32 timer);

} // namespace Delay
} // namespace Wiimmfi
