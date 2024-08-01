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

    SaveExpansionCup() { mData = new Data[GetCupCount()]; }
    virtual u32 GetMagic() { return 'CUPC'; }
    virtual u32 GetRequiredSpace() { return offsetof(RawData, mData) + sizeof(Data) * GetCupCount(); }

    virtual void Init();
    virtual void Read(u8* buffer);
    virtual void Write(u8* buffer);

    u32 GetCupCount() { return CupManager::GetCupListData(CupManager::TRACKS_GP)->cupCount; }
    Data* GetData(u32 cupIdx) { return &mData[cupIdx]; }

    static SaveExpansionCup* GetSection() {
        return (SaveExpansionCup*)SaveManager::GetExpansionSection(SECTION_CUP_COMPLETION);
    }

    Data* mData;
};
