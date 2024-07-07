#include <common/Common.hpp>
#include <nw4r/lyt/material.hpp>

namespace nw4r {
namespace lyt {

class Pane {
public:
    Material* GetMaterial();

    u8 unk[0xB8];
    u8 alpha;
    u8 unk2[0xD8 - 0xB9];
};
size_assert(Pane, 0xD8);

} // namespace lyt
} // namespace nw4r
