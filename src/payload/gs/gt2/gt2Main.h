#pragma once
#include "gt2Buffer.h"
#include "gt2Callback.h"
#include <gs/darray.h>
#include <gs/hashtable.h>

#ifdef __cplusplus
extern "C" {
#endif

struct GTI2Socket {
    int socket;
    u32 ip;
    u16 port;
    PAD(2);

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
size_cassert(GTI2Socket, 0x4C);

struct GTI2Connection {
    u32 ip;
    u16 port;
    u8 aid; // custom field
    PAD(1);

    GTI2Socket* socket;
    GT2SocketState state;
    int initiated;
    int freeAtAcceptReject;
    GT2Result result;
    u32 startTime;
    u32 timeout;
    int callbackLevel;
    GT2Callbacks callbacks;
    char* initialMessage;
    int initialMessageLen;
    void* data;
    GTI2Buffer incomingBuffer;
    GTI2Buffer outgoingBuffer;
    DArray incomingBufferMessages;
    DArray outgoingBufferMessages;
    u16 serialNumber;
    u16 expectedSerialNumber;
    char response[32];
    u32 lastSend;
    u32 challengeTime;
    int pendingAck;
    u32 pendingAckTime;
    DArray sendFilters;
    DArray receiveFilters;
};
size_cassert(GTI2Connection, 0xA0);

GT2Result gt2Connect(GT2Socket socket, GT2Connection* connection, const char* remoteAddress, const char* msg,
                     int msgLen, int timeout, GT2Callbacks* callbacks, int blocking);

GT2Result gt2CreateSocket(GT2Socket* socket, const char* localAddress, int outgoingBufferSize,
                          int incomingBufferSize, GT2SocketErrorCallback callback);

BOOL gt2Accept(GT2Connection connection, GT2Callbacks* callbacks);
void gt2Listen(GT2Socket socket, GT2ConnectAttemptCallback callback);
void gt2Reject(GT2Connection connection, const char* msg, int msgLen);
void gt2SetUnrecognizedMessageCallback(GT2Socket socket, GT2UnrecognizedMessageCallback callback);

#ifdef __cplusplus
}
#endif
