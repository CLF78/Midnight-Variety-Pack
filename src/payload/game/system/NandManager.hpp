#pragma once
#include <egg/core/eggDisposer.hpp>

class NandManager {
public:
    EGG::Disposer disposer;
    virtual ~NandManager();

    bool busy;
    bool _15;
    PAD(2);

    int checkError;
    void* saveBannerTpl;
    void* emptyBuffer;
    void* saveBanner;

    static NandManager* instance;
};
size_assert(NandManager, 0x28);
