#pragma once

namespace nw4r {
namespace snd {
namespace detail {

class Util {
    public:
        struct DataRef {
            u8 refType; // 0 = address, 1 = offset
            u8 dataType;
            u16 reserved;
            u32 value;
        };
        size_assert(DataRef, 0x8);

        template<typename T>
        struct Table {
            u32 count;
            T items[1]; // actually multiple
        };
};

} // namespace detail
} // namespace snd
} // namespace nw4r
