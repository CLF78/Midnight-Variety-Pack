#pragma once

namespace nw4r {
namespace ut {
namespace detail {

struct RuntimeTypeInfo {
    const RuntimeTypeInfo* parentTypeInfo;
};
size_assert(RuntimeTypeInfo, 0x4);

} // namespace detail
} // namespace ut
} // namespace nw4r
