#pragma once
#include "gp.h"
#include "gpiBuffer.h"
#include "gpiCallback.h"
#include "gpiOperation.h"
#include "gpiPeer.h"
#include "gpiProfile.h"
#include <gs/darray.h>

#ifdef __cplusplus
extern "C" {
#endif

struct GPIConnection {
    char errorString[256];
    int infoCaching;
    int infoCachingBuddyOnly;
    int simulation;
    int firewall;

    char nick[31];
    char uniqueNick[21];
    char email[51];
    char password[31];
    PAD(2);

    int sessKey;
    int userId;
    int profileId;
    int partnerId;
    GPICallback callbacks[9];
    int cmSocket;
    int connectState;
    GPIBuffer socketBuffer;
    char* inputBuffer;
    int inputBufferSize;
    GPIBuffer outputBuffer;

    char mHeader[16];
    u16 peerPort;
    PAD(2);

    int nextOperationId;
    int numSearches;
    int lastStatusState;

    u32 hostIp;
    u32 hostPrivateIp;
    u16 queryPort;
    u16 hostPort;
    int sessionFlags;

    char richStatus[256];
    char gameType[33];
    char gameVariant[33];
    char gameMapName[33];
    PAD(1);

    DArray extendedInfoKeys;
    char lastStatusString[256];
    char lastLocationString[256];
    int errorCode;
    int fatalError;
    void* diskCache; // actually FILE
    GPIOperation* operationList;
    GPIProfileList profileList;
    GPIPeer* peerList;
    GPICallbackData* callbackList;
    GPICallbackData* lastCallback;
    GPIBuffer updateProBuffer;
    GPIBuffer updateUiBuffer;
    DArray transfers;
    u32 nextTransferId;
    int productId;
    int namespaceId;
    char loginTicket[25];
    PAD(3);

    int quietModeFlags;
    int kaTransfer;
};
size_cassert(GPIConnection, 0x634);

#ifdef __cplusplus
}
#endif
