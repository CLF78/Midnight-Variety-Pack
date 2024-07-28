#pragma once

namespace nw4r {
namespace snd {
namespace detail {

class MmlParser {
    public:
        void* vtable;
};
size_assert(MmlParser, 0x4);

} // namespace detail
} // namespace snd
} // namespace nw4r
