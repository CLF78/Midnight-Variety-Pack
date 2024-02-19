#include <common/Common.hpp>

namespace Wiimmfi {
namespace Natneg {

    // The structure for DWC_CMD_CONN_FAIL_MTX
    // This command is sent by client A to the host to tell them client B could not complete NATNEG with them
    // The host then sends a NEW_PID_AID command to client B to try and solve the problem quicker
    struct DWCMatchCommandConnFailMtx {
        u32 connFailMtx;
    };

    // Updates the NATNEG timers for Fast NATNEG
    void Calc(bool connectedToHost);

    // Parse custom commands (overrides the DWC function)
    int ProcessRecvMatchCommand(u8 cmd, int profileId, u32 publicIp, u16 publicPort, void* cmdData, int dataLen);

    // Sends a DWC_CMD_CONN_FAIL_MTX command to another client
    // The data required for the command is in the arguments
    void SendConnFailMtxCommand(u32 aidsConnectedToHost, u32 aidsConnectedToMe);

} // namespace Natneg
} // namespace Wiimmfi
