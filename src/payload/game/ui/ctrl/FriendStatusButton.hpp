#include <common/Common.hpp>
#include <game/system/Mii.hpp>
#include <game/ui/PushButton.hpp>

class FriendStatusButton : public PushButton {
public:
    virtual ~FriendStatusButton();

    virtual void initSelf();
    virtual void calcSelf();

    virtual const TypeInfo* getTypeInfo() const;
    virtual const char* getTypeName() const;

    virtual void onSelect();
    virtual void onDeselect();

    static const char* animNames[11];

    MiiGroup* miiGroup;
    int friendIndex;
};
size_assert(FriendStatusButton, 0x25C);
