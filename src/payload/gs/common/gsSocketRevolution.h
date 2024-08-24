#pragma once
#include <revolutionex/so/SOBasic.h>

#ifdef __cplusplus
extern "C" {
#endif

int bind(int sock, SOSockAddrIn* addr, int len);
SOHostEnt* gethostbyname(const char* name);
int socket(int protocolFamily, int type, int protocol);
int sendto(int sock, const char* buf, int len, int flags, const SOSockAddrIn* addr, int tolen);

#ifdef __cplusplus
}
#endif
