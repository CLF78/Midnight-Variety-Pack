#pragma once

namespace nw4r {
namespace ef {

class HandleBase {
public:
    void* GetPtr() const;
    bool IsValid() const;

    u32 objId;
    void* object;
};
size_assert(HandleBase, 0x8);

} // namespace ef
} // namespace nw4r
