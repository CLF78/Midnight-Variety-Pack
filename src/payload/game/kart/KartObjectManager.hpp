#pragma once
#include "KartObject.hpp"
#include <egg/core/eggDisposer.hpp>

class KartObjectManager : public EGG::Disposer {
public:
    KartObjectManager();
    virtual ~KartObjectManager();

    KartObject* getKartObject(u32 idx) const;
    void init();
    void update();

    static KartObjectManager* CreateInstance();
    static void DestroyInstance();

    EGG::Disposer verifyDisposer;
    KartObject** players;
    UNK(0x38 - 0x24);

    static KartObjectManager* instance;
    static nw4r::ut::List kartObjects;
    static void* curKartClipInfo;
};
size_assert(KartObjectManager, 0x38);
