#include <common/Common.hpp>

class AudioHandleHolder {
public:
    static AudioHandleHolder* instance;

    void prepare(u32 soundId, bool unk);
    void prepareSpecial(u32 soundId, bool unk);
};
