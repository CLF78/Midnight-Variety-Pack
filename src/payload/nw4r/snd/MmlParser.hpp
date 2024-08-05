#pragma once

namespace nw4r {
namespace snd {
namespace detail {

class MmlSeqTrack;

class MmlParser {
public:
    virtual void CommandProc(MmlSeqTrack* track, u32 command, s32 commandArg1, s32 commandArg2) const;
    virtual void NoteOnCommandProc(MmlSeqTrack* track, int key, int velocity, s32 length, bool tieFlag) const;
};
size_assert(MmlParser, 0x4);

} // namespace detail
} // namespace snd
} // namespace nw4r
