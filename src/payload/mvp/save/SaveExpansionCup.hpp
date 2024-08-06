#pragma once
#include "SaveExpansionSection.hpp"
#include <game/system/SaveManager.hpp>
#include <mvp/cup/CupManager.hpp>

class SaveExpansionCup : public SaveExpansionSection {
public:

    struct Data {
        void Init() { mCompleted = false; mRank = 0xFF;}
        void Set(bool completed, u8 rank) { mCompleted = completed; mRank = rank; }

        bool mCompleted;
        u8 mRank;
    };

    struct RawData {
        u32 mEntryCount;
        Data mData[];
    };

    SaveExpansionCup() : mData(new Data[GetEntryCount()]) {}
    virtual u32 GetMagic() const { return 'CUPC'; }
    virtual u32 GetRequiredSpace() const { return offsetof(RawData, mData) + sizeof(Data) * GetEntryCount(); }

    virtual void Init();
    virtual void Read(u8* buffer);
    virtual void Write(u8* buffer);

    static u32 GetEntryCount() { return CupManager::GetCupListData(CupManager::TRACKS_GP)->cupCount; }
    Data* GetData(u32 cupIdx) const { return &mData[cupIdx]; }

    static SaveExpansionCup* GetSection() {
        return (SaveExpansionCup*)SaveManager::GetExpansionSection(SECTION_CUP_COMPLETION);
    }

    Data* mData;
};
