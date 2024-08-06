#pragma once
#include "SaveExpansionSection.hpp"
#include <game/system/SaveManager.hpp>
#include <mvp/cup/CupManager.hpp>

class SaveExpansionRating : public SaveExpansionSection {
public:

    struct Data {
        void Init() { mRating = 1000; }
        void Set(u16 value) { mRating = value; }

        u16 mRating;
    };

    struct RawData {
        u32 mEntryCount;
        Data mData[];
    };

    SaveExpansionRating() : mData(new Data[GetEntryCount()]) {  }
    virtual u32 GetMagic() const { return 'RTNG'; }
    virtual u32 GetRequiredSpace() const { return offsetof(RawData, mData) + sizeof(Data) * GetEntryCount(); }

    virtual void Init();
    virtual void Read(u8* buffer);
    virtual void Write(u8* buffer);

    static u32 GetEntryCount() { return CupManager::TRACKS_TOTAL_COUNT; }
    Data* GetData(u32 idx) const { return &mData[idx]; }

    static SaveExpansionRating* GetSection() {
        return (SaveExpansionRating*)SaveManager::GetExpansionSection(SECTION_RATING);
    }

    Data* mData;
};
