#pragma once
#include "SaveExpansionSection.hpp"
#include <game/system/Identifiers.hpp>
#include <game/system/RaceConfig.hpp>
#include <game/system/SaveManager.hpp>
#include <mvp/cup/CupManager.hpp>

class SaveExpansionDrift : public SaveExpansionSection {
public:
    struct Data {

        void Init() {
            for (u32 i = 0; i < ARRAY_SIZE(mTransmissions); i++) {
                mTransmissions[i] = RaceConfig::Player::TRANSMISSION_DEFAULT;
            }
        }

        void Set(u8 vehicle, u8 transmission) { mTransmissions[vehicle] = transmission; }
        u8 Get(u8 vehicle);

        u8 mTransmissions[VEHICLE_COUNT];
    };

    struct RawData {
        u32 mEntryCount;
        Data mData[];
    };

    SaveExpansionDrift() : mData(new Data[GetEntryCount()]) {}
    virtual u32 GetMagic() const { return 'DRFT'; }
    virtual u32 GetRequiredSpace() const { return offsetof(RawData, mData) + sizeof(Data) * GetEntryCount(); }

    virtual void Init();
    virtual void Read(u8* buffer);
    virtual void Write(u8* buffer);

    static u32 GetEntryCount() { return 4; }
    Data* GetData(u32 idx) const { return &mData[idx]; }

    static SaveExpansionDrift* GetSection() {
        return (SaveExpansionDrift*)SaveManager::GetExpansionSection(SECTION_DRIFT);
    }

    Data* mData;
};
