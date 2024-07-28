#pragma once
#include <common/Common.h>
#include <revolutionex/so/SOBasic.h>

#ifdef __cplusplus
extern "C" {
#endif

SOHostEnt* gethostbyname(const char* name);
int socket(int protocolFamily, int type, int protocol);
int sendto(int sock, const char* buf, int len, int flags, const SOSockAddrIn* addr, int tolen);

#ifdef __cplusplus
}
#endif
