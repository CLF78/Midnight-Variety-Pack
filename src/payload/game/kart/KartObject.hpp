#pragma once
#include "KartObjectProxy.hpp"
#include "KartSettings.hpp"

class KartObject : public KartObjectProxy {
public:
    virtual ~KartObject();
    virtual void InitSub();
    virtual void CreateBody();
    virtual void InitWheels(void* brresHandle);
    virtual void CreateSound();

    s16 getDriftState() const;

    static KartObject* Create(u8 playerIdx, int kart, int character, bool isBike);

    KartSub* kartSub;
    KartSettings* kartSettings;
    void* driver;
    KartPointers pointers;
};
size_assert(KartObject, 0x80);
