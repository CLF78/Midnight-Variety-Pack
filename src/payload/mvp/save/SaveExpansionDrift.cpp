#include "SaveExpansionDrift.hpp"

u8 SaveExpansionDrift::Data::Get(u8 vehicle) {
    if (mTransmissions[vehicle] != RaceConfig::Player::TRANSMISSION_DEFAULT) {
        return mTransmissions[vehicle];
    }

    static const u8 sDefaultTransmissions[VEHICLE_COUNT] = {
        RaceConfig::Player::TRANSMISSION_OUTSIDE, RaceConfig::Player::TRANSMISSION_OUTSIDE,
        RaceConfig::Player::TRANSMISSION_OUTSIDE, RaceConfig::Player::TRANSMISSION_OUTSIDE,
        RaceConfig::Player::TRANSMISSION_OUTSIDE, RaceConfig::Player::TRANSMISSION_OUTSIDE,
        RaceConfig::Player::TRANSMISSION_OUTSIDE, RaceConfig::Player::TRANSMISSION_OUTSIDE,
        RaceConfig::Player::TRANSMISSION_OUTSIDE, RaceConfig::Player::TRANSMISSION_OUTSIDE,
        RaceConfig::Player::TRANSMISSION_OUTSIDE, RaceConfig::Player::TRANSMISSION_OUTSIDE,
        RaceConfig::Player::TRANSMISSION_OUTSIDE, RaceConfig::Player::TRANSMISSION_OUTSIDE,
        RaceConfig::Player::TRANSMISSION_OUTSIDE, RaceConfig::Player::TRANSMISSION_OUTSIDE,
        RaceConfig::Player::TRANSMISSION_OUTSIDE, RaceConfig::Player::TRANSMISSION_OUTSIDE,

        RaceConfig::Player::TRANSMISSION_OUTSIDE, RaceConfig::Player::TRANSMISSION_OUTSIDE,
        RaceConfig::Player::TRANSMISSION_OUTSIDE, RaceConfig::Player::TRANSMISSION_INSIDE,
        RaceConfig::Player::TRANSMISSION_INSIDE,  RaceConfig::Player::TRANSMISSION_INSIDE,
        RaceConfig::Player::TRANSMISSION_OUTSIDE, RaceConfig::Player::TRANSMISSION_OUTSIDE,
        RaceConfig::Player::TRANSMISSION_OUTSIDE, RaceConfig::Player::TRANSMISSION_INSIDE,
        RaceConfig::Player::TRANSMISSION_OUTSIDE, RaceConfig::Player::TRANSMISSION_OUTSIDE,
        RaceConfig::Player::TRANSMISSION_INSIDE,  RaceConfig::Player::TRANSMISSION_INSIDE,
        RaceConfig::Player::TRANSMISSION_INSIDE,  RaceConfig::Player::TRANSMISSION_INSIDE,
        RaceConfig::Player::TRANSMISSION_INSIDE,  RaceConfig::Player::TRANSMISSION_OUTSIDE,
    };

    return sDefaultTransmissions[vehicle];
}

void SaveExpansionDrift::Init() {

    // Reset the data
    for (u32 i = 0; i < GetEntryCount(); i++) {
        mData[i].Init();
    }
}

void SaveExpansionDrift::Read(u8* buffer) {

    // Get the raw data
    RawData* data = (RawData*)buffer;

    // Fill it in until either our data is filled or the save data runs out
    for (u32 i = 0; i < data->mEntryCount && i < GetEntryCount(); i++) {
        mData[i] = data->mData[i];
    }
}

void SaveExpansionDrift::Write(u8* buffer) {

    // Get the raw data
    RawData* data = (RawData*)buffer;

    // Set the entry count
    data->mEntryCount = GetEntryCount();

    // Copy the data over
    for (u32 i = 0; i < data->mEntryCount; i++) {
        data->mData[i] = mData[i];
    }
}
