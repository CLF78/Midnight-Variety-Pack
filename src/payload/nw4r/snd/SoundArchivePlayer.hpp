#pragma once
#include "DisposeCallbackManager.hpp"
#include "MmlParser.hpp"
#include "MmlSeqTrackAllocator.hpp"
#include "NoteOnCallback.hpp"
#include "SeqTrackAllocator.hpp"
#include "SoundActor.hpp"
#include "SoundArchive.hpp"
#include "SoundHandle.hpp"
#include "SoundInstanceManager.hpp"
#include "SoundMemoryAllocatable.hpp"
#include "SoundPlayer.hpp"
#include "SoundStartable.hpp"
#include "StrmChannel.hpp"
#include "Util.hpp"
#include "WsdPlayer.hpp"

namespace nw4r {
namespace snd {

class SoundArchivePlayer : public detail::DisposeCallback, public SoundStartable {
public:
    class SeqNoteOnCallback : public detail::NoteOnCallback {
    public:
        SoundArchivePlayer* mSoundArchivePlayer;
    };
    size_assert(SeqNoteOnCallback, 0x8);

    class WsdCallback : public detail::WsdPlayer::WsdCallback {
    public:
        SoundArchivePlayer* mSoundArchivePlayer;
    };
    size_assert(WsdCallback, 0x8);

    struct GroupAddress {
        const void* address;
        const void* waveDataAddress;
    };
    size_assert(GroupAddress, 0x8);

    struct FileAddress {
        const void* address;
        const void* waveDataAddress;
    };
    size_assert(FileAddress, 0x8);

    bool IsAvailable() const;
    bool LoadGroup(ulong groupId, SoundMemoryAllocatable* allocator, ulong loadBlockSize = 0);
    StartResult detail_SetupSoundImpl(SoundHandle* handle, ulong soundId,
                                      detail::BasicSound::AmbientInfo* ambientArgInfo, SoundActor* actor,
                                      bool holdFlag, const StartInfo* startInfo);

    const void* detail_GetFileAddress(u32 fileId) const;
    const void* detail_GetFileWaveDataAddress(u32 fileId) const;

    ulong GetRequiredMemSize(const SoundArchive* archive);
    bool SetupMram(const SoundArchive* archive, void* buffer, size_t size);
    void InvalidateData(const void* start, const void* end);

    StartResult PrepareStrmImpl(detail::BasicSound* sound, const SoundArchive::SoundInfo* commonInfo,
                                const SoundArchive::StrmSoundInfo* info,
                                SoundStartable::StartInfo::StartOffsetType startOffsetType, int startOffset);

    ut::FileStream** soundStreams() const; // custom function

    const SoundArchive* soundArchive;
    detail::Util::Table<GroupAddress>* groupTable;
    detail::Util::Table<FileAddress>* fileTable;

    // Callbacks
    SeqNoteOnCallback seqCallback;
    WsdCallback wsdCallback;
    detail::SeqTrackAllocator* seqTrackAllocator;

    // Sound Player
    ulong soundPlayerCount;
    SoundPlayer* soundPlayers;

    // Manager/allocator
    detail::SoundInstanceManager seqSoundInstanceManager;
    detail::SoundInstanceManager strmSoundInstanceManager;
    detail::SoundInstanceManager waveSoundInstanceManager;
    detail::MmlSeqTrackAllocator mmlSeqTrackAllocator; // default sequence track allocator
    detail::StrmBufferPool strmBufferPool;

    // Default sequence spacer
    detail::MmlParser mmlParser;

    // Record the buffer information
    void* setupBufferAddress;
    ulong setupBufferSize;
};
size_assert(SoundArchivePlayer, 0xE0);

} // namespace snd
} // namespace nw4r
