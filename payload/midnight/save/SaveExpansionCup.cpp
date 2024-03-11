#include <common/Common.hpp>
#include <midnight/save/SaveExpansionCup.hpp>

void SaveExpansionCup::Init() {

    // Reset the data
    for (int i = 0; i < SaveExpansionCup::GetCupCount(); i++) {
        mData[i].Init();
    }
}

void SaveExpansionCup::Read(u8* buffer) {

    // Get the raw data
    RawData* data = (RawData*)buffer;

    // Fill it in until either our data is filled or the save data runs out
    for (int i = 0; i < data->mEntryCount && i < GetCupCount(); i++) {
        mData[i] = data->mData[i];
    }
}

void SaveExpansionCup::Write(u8* buffer) {

    // Get the raw data
    RawData* data = (RawData*)buffer;

    // Set the entry count
    data->mEntryCount = GetCupCount();

    // Copy the data over
    for (int i = 0; i < data->mEntryCount; i++) {
        data->mData[i] = mData[i];
    }
}
