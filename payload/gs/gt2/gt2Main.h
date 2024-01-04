#include <common/Common.h>
#include <gs/darray.h>
#include <gs/hashtable.h>
#include <gs/gt2/gt2Callback.h>

#ifdef __cplusplus
extern "C" {
#endif

struct GTI2Socket {
    int socket;
    u32 ip;
    u16 port;
    // 2 bytes padding

    HashTable connections;
    DArray closedConnections;
    int close;
    int error;
    int callbackLevel;

    GT2ConnectAttemptCallback connectAttemptCallback;
    GT2SocketErrorCallback socketErrorCallback;
    GT2DumpCallback sendDumpCallback;
    GT2DumpCallback receiveDumpCallback;
    GT2UnrecognizedMessageCallback unrecognizedMsgCallback;

    void* data;
    u32 sendBufferSize;
    u32 recvBufferSize;

    GT2Protocol protocolType;
    int protocolOffset;
    int broadcastEnabled;
};

#ifdef __cplusplus
}
#endif
