#pragma once
#include <common/Common.hpp>

namespace nw4r {
namespace ut {

struct BinaryFileHeader {
    u32 signature;
    u16 byteOrder;
    u16 version;
    u32 fileSize;
    u16 headerSize;
    u16 dataBlocks;
};
size_assert(BinaryFileHeader, 0x10);

} // namespace ut
} // namespace nw4r
