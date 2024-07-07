#include <common/Common.hpp>

struct TypeInfo {
    TypeInfo(TypeInfo* parent) : base(parent) {}
    TypeInfo* base;
};
size_assert(TypeInfo, 0x4);
