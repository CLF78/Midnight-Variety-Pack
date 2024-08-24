#pragma once

namespace nw4r {
namespace ut {

struct Link {
    void* prev;
    void* next;
};
size_assert(Link, 0x8);

struct List {
    void* head;
    void* tail;
    u16 count;
    u16 offset;
};
size_assert(List, 0xC);

struct LinkListImpl {
    u32 count;
    Link node;
};
size_assert(LinkListImpl, 0xC);

} // namespace ut
} // namespace nw4r
