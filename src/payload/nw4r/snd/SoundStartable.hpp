#pragma once

namespace nw4r {
namespace snd {

class SoundStartable {
public:
    enum StartResult {
        START_SUCCESS,
        START_ERR_LOW_PRIORITY,
        START_ERR_INVALID_LABEL_STRING,
        START_ERR_INVALID_SOUNDID,
        START_ERR_NOT_DATA_LOADED,
        START_ERR_NOT_ENOUGH_PLAYER_HEAP,
        START_ERR_CANNOT_OPEN_FILE,
        START_ERR_NOT_AVAILABLE,
        START_ERR_CANNOT_ALLOCATE_TRACK, // Unused
        START_ERR_NOT_ENOUGH_INSTANCE,
        START_ERR_INVALID_PARAMETER,
        START_ERR_INVALID_SEQ_START_LOCATION_LABEL,
        START_ERR_USER = 128,
        START_ERR_UNKNOWN = 255,
    };

    struct StartInfo {
        enum EnableFlagBit {
            ENABLE_START_OFFSET = BIT_FLAG(0),
            ENABLE_PLAYER_ID = BIT_FLAG(1),
            ENABLE_PLAYER_PRIORITY = BIT_FLAG(2),
            ENABLE_ACTOR_PLAYER_ID = BIT_FLAG(3),
            ENABLE_SEQ_SOUND_INFO = BIT_FLAG(4),
        };

        enum StartOffsetType {
            START_OFFSET_TYPE_MILLISEC,
            START_OFFSET_TYPE_TICK,
            START_OFFSET_TYPE_SAMPLE,
        };

        struct SeqSoundInfo {
            const void* seqDataAddress;
            const char* startLocationLabel;
        };
        size_assert(SeqSoundInfo, 0x8);

        ulong enableFlag;
        StartOffsetType startOffsetType;
        int startOffset;
        ulong playerId;
        int playerPriority;
        int actorPlayerId;
        SeqSoundInfo seqSoundInfo;
    };
    size_assert(StartInfo, 0x20);

    virtual ~SoundStartable();
};
size_assert(SoundStartable, 0x4);

} // namespace snd
} // namespace nw4r
