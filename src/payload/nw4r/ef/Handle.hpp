#pragma once
#include <common/Common.hpp>

namespace nw4r {
namespace ef {

struct HandleBase {
    HandleBase();
    HandleBase(const HandleBase& hnd);
    void* GetPtr() const;
    bool IsValid() const;
    HandleBase& operator=(const HandleBase& other);
    HandleBase& operator=(void* obj);
    u32 objId;
    void* object;
};

}//namespace ef
}//namespace nw4r
