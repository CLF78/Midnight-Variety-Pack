#pragma once
#include <common/Common.hpp>
#include <game/kart/KartStats.hpp>
#include <game/kart/KartObjectProxy.hpp>
#include <game/kart/KartParam.hpp>
#include <game/kart/KartSettings.hpp>

class KartObject : public KartObjectProxy {
public:

    u16 getDriftState() const;

    virtual ~KartObject();
    virtual void InitSub();
    virtual void CreateBody();
    virtual void InitWheels(void* brresHandle);
    virtual void CreateSound();
    static KartObject* Create(u8 playerIdx, int kart, int character, bool isBike);

    KartSub* kartSub;
    KartSettings* kartSettings;
    void* driver;
    KartPointers pointers;
};
