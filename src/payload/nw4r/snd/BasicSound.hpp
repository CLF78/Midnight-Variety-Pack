#pragma once

namespace nw4r {
namespace snd {
namespace detail {

struct SoundActorParam {
    f32 volume;
    f32 pitch;
    f32 pan;
};
size_assert(SoundActorParam, 0xC);

class BasicSound {
public:

    class AmbientParamUpdateCallback {
    public:
        virtual ~AmbientParamUpdateCallback();
    };
    size_assert(AmbientParamUpdateCallback, 0x4);

    class AmbientArgUpdateCallback {
    public:
        virtual ~AmbientArgUpdateCallback();
    };
    size_assert(AmbientArgUpdateCallback, 0x4);

    class AmbientArgAllocaterCallback {
    public:
        virtual ~AmbientArgAllocaterCallback();
    };
    size_assert(AmbientArgAllocaterCallback, 0x4);

    struct AmbientInfo {
        AmbientParamUpdateCallback* paramUpdateCallback;
        AmbientArgUpdateCallback* argUpdateCallback;
        AmbientArgAllocaterCallback* argAllocaterCallback;
        void* arg;
        size_t argSize;
    };
    size_assert(AmbientInfo, 0x14);

    void SetId(ulong id);
    static s32 GetAmbientPriority(AmbientInfo* info, ulong id);
};

} // namespace detail
} // namespace snd
} // namespace nw4r
