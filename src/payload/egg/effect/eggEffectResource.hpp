#pragma once
#include <egg/core/eggDisposer.hpp>
#include <nw4r/ut/utList.hpp>

namespace EGG {

class EffectResource : public Disposer {
public:
    EffectResource(void* breff, void* breft);
    virtual ~EffectResource();
    virtual void setResource(void* breff, void* breft);
    virtual void resetResource();
    void initialize();

    void* breff;
    void* breft;
    nw4r::ut::Link resourceLink;

    static nw4r::ut::List effectResList;
};
size_assert(EffectResource, 0x20);

} // namespace EGG
