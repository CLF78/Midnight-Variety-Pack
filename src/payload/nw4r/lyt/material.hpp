#pragma once
#include "texmap.hpp"

namespace nw4r {
namespace lyt {

class Material {
public:
    TexMap* GetTexMapAry();

    UNK(0x3C);
    u32 flags;
};

} // namespace lyt
} // namespace nw4r
