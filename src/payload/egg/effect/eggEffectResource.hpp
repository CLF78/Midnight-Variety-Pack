#pragma once
#include <egg/core/eggDisposer.hpp>
#include <nw4r/ut/utList.hpp>

using namespace nw4r;
namespace EGG {
class EffectResource : EGG::Disposer {
public:
    static nw4r::ut::List effectResList;
    EffectResource(void* breff, void* breft);
    virtual ~EffectResource();
    virtual void setResource(void* breff, void* breft);
    virtual void resetResource();
    void initialize();
    void* breff;
    void* breft; 
    ut::Link resourceLink;

};
size_assert(EGG::EffectResource, 0x20);
}
