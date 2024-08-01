#pragma once
#include "PushButton.hpp"
#include <game/system/Mii.hpp>

class FriendStatusButton : public PushButton {
public:
    virtual ~FriendStatusButton();

    virtual void initSelf();
    virtual void calcSelf();

    RUNTIME_TYPE_INFO_NODECL;
    RUNTIME_TYPENAME_INFO_NODECL;

    virtual void onSelect();
    virtual void onDeselect();

    static const char* animNames[11];

    MiiGroup* miiGroup;
    int friendIndex;
};
size_assert(FriendStatusButton, 0x25C);
