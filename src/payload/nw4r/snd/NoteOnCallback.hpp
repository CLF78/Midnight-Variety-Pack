#pragma once

namespace nw4r {
namespace snd {
namespace detail {

class NoteOnCallback {
public:
    virtual ~NoteOnCallback();
};
size_assert(NoteOnCallback, 0x4);

} // namespace detail
} // namespace snd
} // namespace nw4r
