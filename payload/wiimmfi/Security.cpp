#include <common/Common.hpp>
#include <game/net/RKNetController.hpp>
#include <game/net/packet/RKNetRoomPacket.hpp>
#include <game/net/packet/RKNetSelectPacket.hpp>
#include <wiimmfi/Security.hpp>

namespace Wiimmfi {
namespace Security {

bool IsPacketSectionSizeValid(int section, u32 sectionSize) {

    // Skip empty sections
    if (sectionSize == 0)
        return true;

    // The check depends on each section
    u32 destSize = RKNetController::packetBufferSizes[section];
    switch (section) {

        // For EVENT packets, ensure the size fits the buffer as it is variable
        case RKNetRACEPacketHeader::EVENT:
            return sectionSize <= destSize;

        // For ROOM/SELECT packets, ensure the size matches one of the two possible values
        case RKNetRACEPacketHeader::ROOM_SELECT:
            return (sectionSize == sizeof(RKNetROOMPacket) || sectionSize == sizeof(RKNetSELECTPacket));

        // For RACEDATA and ITEM packets, ensure the size is either half or the full buffer size
        case RKNetRACEPacketHeader::RACEDATA:
        case RKNetRACEPacketHeader::ITEM:
            return (sectionSize == destSize / 2 || sectionSize == destSize);

        // For all other sections, match exactly the default size
        default:
            return sectionSize == destSize;
    }
}

// TODO implement this after all the packet structures are defined
bool IsPacketSectionDataValid(int section, u8* sectionData) {
    return true;
}

// Code ported from WiiLink24's WFC patcher
bool ValidateRACEPacket(u32 aid, RKNetRACEPacketHeader* data, u32 dataLength) {

    // Bail if the packet doesn't even include a full header
    if (dataLength < sizeof(RKNetRACEPacketHeader))
        return false;

    u32 expectedPacketSize = 0;
    for (int i = 0; i < RKNetRACEPacketHeader::SECTION_COUNT; i++) {
        u8 sectionSize = data->sizes[i];

        // Verify the section size is valid
        if (!IsPacketSectionSizeValid(i, sectionSize))
            return false;

        // Verify the section data is valid
        if (!IsPacketSectionDataValid(i, (u8*)data + expectedPacketSize))
            return false;

        expectedPacketSize += sectionSize;
    }

    // Ensure the declared sizes match the cumulative size
    return dataLength >= expectedPacketSize;
}

} // namespace Security
} // namespace Wiimmfi
