#pragma once
#include "KartObject.hpp"
#include <egg/core/eggDisposer.hpp>

class KartObjectManager : public EGG::Disposer {
public:
    static KartObjectManager* instance;
    static KartObjectManager* CreateInstance();
    static void DestroyInstance();
    static nw4r::ut::List kartObjects;
    static void* curKartClipInfo;

    virtual ~KartObjectManager();
    KartObjectManager();
    KartObject* getKartObject(u32 idx) const;
    void init();
    void update();

    EGG::Disposer disposer;
    KartObject** players;
};
