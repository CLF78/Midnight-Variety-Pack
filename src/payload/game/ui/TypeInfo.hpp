#include <common/Common.hpp>

struct TypeInfo {
    TypeInfo(TypeInfo* parent) : base(parent) {}
    TypeInfo* base;
};
size_assert(TypeInfo, 0x4);

#define RUNTIME_TYPE_INFO \
    virtual const TypeInfo* getTypeInfo() const { return &typeInfo; } \
    static const TypeInfo typeInfo

#define RUNTIME_TYPE_INFO_NODECL \
    virtual const TypeInfo* getTypeInfo() const; \
    static const TypeInfo typeInfo

#define RUNTIME_TYPENAME_INFO(type) \
    virtual const char* getTypeName() const { return STRINGIFY(type); }

#define RUNTIME_TYPENAME_INFO_NODECL \
    virtual const char* getTypeName() const

#define RUNTIME_TYPE_INFO_DEFINE(type, parentType) \
const TypeInfo type::typeInfo = TypeInfo(parentType::typeInfo)

#define RUNTIME_TYPE_INFO_DEFINE_ROOT(type) \
const TypeInfo type::typeInfo = TypeInfo(nullptr)
