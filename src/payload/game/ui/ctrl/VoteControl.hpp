#pragma once
#include "LayoutUIControl.hpp"
#include <game/system/Mii.hpp>

class VoteControl : public LayoutUIControl {
public:
    virtual ~VoteControl();

    virtual void initSelf();
    virtual void calcSelf();

    RUNTIME_TYPE_INFO_NODECL;
    RUNTIME_TYPENAME_INFO_NODECL;

    void setData(bool isRandom, u32 bmgId, MiiGroup* miiGroup, int playerIdx, bool isLocal, int team);

    bool isRandomVote;
};
size_assert(VoteControl, 0x178);
