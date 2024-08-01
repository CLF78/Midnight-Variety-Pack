#include "dwc_common.h"
#include "dwc_main.h"
#include "dwc_report.h"
#include <gs/gt2/gt2Main.h>
#include <gs/gt2/gt2Utility.h>
#include <wiimmfi/ConnectionMatrix.hpp>
#include <wiimmfi/Natneg.hpp>
#include <wiimmfi/Port.hpp>

/////////////////
// Fast NATNEG //
/////////////////

// Replace ConnectedCallback to accept incoming connections in more cases
// Credits: Wiimmfi
REPLACE void DWC_InitFriendsMatch(void* unused, DWCUserData* userdata, int productID, const char* gameName,
                                  const char* secretKey, int sendBufSize, int recvBufSize,
                                  DWCFriendData* friendList, int friendListLen) {
    REPLACED(unused, userdata, productID, gameName, secretKey, sendBufSize, recvBufSize, friendList, friendListLen);
    stpDwcCnt->gt2Callbacks.connectedCb = &Wiimmfi::Natneg::ConnectedCallback;
}

// Update NATNEG timers
// Credits: Wiimmfi
REPLACE void DWCi_MatchedCallback(DWCError error, BOOL cancel, BOOL self, BOOL isServer, int index, void* param) {
    REPLACED(error, cancel, self, isServer, index, param);
    Wiimmfi::Natneg::CalcTimers(self);
}

////////////////////////////////////////
// Fast NATNEG / Wiimmfi Port Binding //
////////////////////////////////////////

// Use the server-provided port for GT2 sockets and replace ConnectAttemptCallback
// Credits: NitroDWC, Wiimmfi
REPLACE GT2Result DWCi_GT2Startup() {

    // Check if the socket was already made
    if (stpDwcCnt->gt2Socket) {
        DWC_Printf(DWC_REPORT_WARNING, "GT2Socket already created\n");
        return GT2_RESULT_SUCCESS;
    }

    // Get the port
    u16 port = (Wiimmfi::Port::sPort) ? Wiimmfi::Port::sPort : (u16)(0xC000 + DWCi_GetMathRand32(0x4000));
    DWC_Printf(DWC_REPORT_MATCH_NN, "Got private port %d\n", port);

    // Create the socket
    GT2Result gt2Result = gt2CreateSocket(&stpDwcCnt->gt2Socket,
                                          gt2AddressToString(0, port, nullptr),
                                          stpDwcCnt->gt2SendBufSize,
                                          stpDwcCnt->gt2RecvBufSize,
                                          DWCi_GT2SocketErrorCallback);

    // Set error code if necessary
    if (DWCi_HandleGT2Error(gt2Result))
        return gt2Result;

    // Set callbacks
    gt2Listen(stpDwcCnt->gt2Socket, Wiimmfi::Natneg::ConnectAttemptCallback);
    gt2SetUnrecognizedMessageCallback(stpDwcCnt->gt2Socket, DWCi_GT2UnrecognizedMessageCallback);
    return gt2Result;
}

/////////////////////////////////////
// Fast NATNEG / Wiimmfi Telemetry //
/////////////////////////////////////

// Update the connection matrix when a connection is closed
// Credits: Wiimmfi
REPLACE void DWCi_GT2ClosedProcess(GT2Connection connection, GT2CloseReason reason, BOOL isPseudClose, u8 closeAid) {
    REPLACED(connection, reason, isPseudClose, closeAid);
    Wiimmfi::ConnectionMatrix::Update();
}
