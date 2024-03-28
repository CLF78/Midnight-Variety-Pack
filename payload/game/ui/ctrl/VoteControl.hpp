#include <common/Common.hpp>
#include <game/system/Mii.hpp>
#include <game/ui/LayoutUIControl.hpp>

class VoteControl : public LayoutUIControl {
public:
    virtual ~VoteControl();

    virtual void initSelf();
    virtual void calcSelf();

    virtual const TypeInfo* getTypeInfo() const;
    virtual const char* getTypeName() const;

    void setData(bool isRandom, u32 bmgId, MiiGroup* miiGroup, int playerIdx, bool isLocal, int team);

    bool isRandomVote;
};
size_assert(VoteControl, 0x178);
