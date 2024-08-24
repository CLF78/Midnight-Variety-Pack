#include "SaveExpansionRating.hpp"

void SaveExpansionRating::Init() {

    // Reset the data
    for (u32 i = 0; i < GetEntryCount(); i++) {
        mData[i].Init();
    }
}

void SaveExpansionRating::Read(u8* buffer) {

    // Get the raw data
    RawData* data = (RawData*)buffer;

    // Fill it in until either our data is filled or the save data runs out
    for (u32 i = 0; i < data->mEntryCount && i < GetEntryCount(); i++) {
        mData[i] = data->mData[i];
    }
}

void SaveExpansionRating::Write(u8* buffer) {

    // Get the raw data
    RawData* data = (RawData*)buffer;

    // Set the entry count
    data->mEntryCount = GetEntryCount();

    // Copy the data over
    for (u32 i = 0; i < data->mEntryCount; i++) {
        data->mData[i] = mData[i];
    }
}
