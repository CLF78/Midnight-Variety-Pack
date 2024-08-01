#pragma once
#include <nw4r/ut/utList.hpp>

namespace EGG {

// Forward declaration
class Heap;

class Disposer {
public:
    virtual ~Disposer();
    Heap* mContainHeap;
    nw4r::ut::Link mLink;
};
size_assert(Disposer, 0x10);

template<class T>
class TDisposer: public Disposer {
public:
    virtual ~TDisposer() {
        T::sInstance = nullptr;
        delete(t);
        Disposer::~Disposer();
    };
};

} // namespace EGG
