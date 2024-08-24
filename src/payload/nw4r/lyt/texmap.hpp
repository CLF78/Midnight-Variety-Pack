#pragma once

namespace nw4r {
namespace lyt {

class TexMap {
public:
    void ReplaceImage(void* image, u32 id); // Actually TPLDescriptor* but i cannot be arsed

    UNK(0x1C);
};
size_assert(TexMap, 0x1C);

} // namespace lyt
} // namespace nw4r
