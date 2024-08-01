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
                const SoundArchivePlayer& mSoundArchivePlayer;
        };
        size_assert(SeqNoteOnCallback, 0x8);

        class WsdCallback : public detail::WsdPlayer::WsdCallback {
            public:
                const SoundArchivePlayer& mSoundArchivePlayer;
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

        ulong GetRequiredMemSize(const SoundArchive* archive);

        bool IsAvailable() const;

        StartResult detail_SetupSoundImpl(SoundHandle* handle,
                                          ulong soundId,
                                          detail::BasicSound::AmbientInfo* ambientArgInfo,
                                          SoundActor* actor,
                                          bool holdFlag,
                                          const StartInfo* startInfo);

        bool LoadGroup(ulong groupId, SoundMemoryAllocatable* allocater, ulong loadBlockSize = 0);

        const void* detail_GetFileAddress(u32 fileId) const;
        const void* detail_GetFileWaveDataAddress(u32 fileId) const;

        void InvalidateData(const void* start, const void* end);

        StartResult PrepareStrmImpl(detail::BasicSound* sound,
                                    const SoundArchive::SoundInfo* commonInfo,
                                    const SoundArchive::StrmSoundInfo* info,
                                    SoundStartable::StartInfo::StartOffsetType startOffsetType,
                                    int startOffset);

        bool SetupMram(const SoundArchive* archive, void* buffer, size_t size);

        ut::FileStream** soundStreams(); // custom function

        const SoundArchive* soundArchive;
        detail::Util::Table<GroupAddress>* groupTable;
        detail::Util::Table<FileAddress>* fileTable;

        // Callback
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
