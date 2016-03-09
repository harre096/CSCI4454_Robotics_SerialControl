#include <cstdio>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "commbase.h"

#ifndef COMMUTIL_H_INCLUDED
#define COMMUTIL_H_INCLUDED

#define SOCKET int
#define SOCKADDR struct sockaddr
#define SOCKET_ERROR -1
#define closesocket close
#define INVALID_SOCKET -1

enum MsgEnum{Exit,OpenCOMPort,MotorCommand,Terminate,Ignore,Unrecognized};

int readIndefiniteFromSocket(char*,int = 30);
void openCOMPort(const char*);
int compStr(const char*,const char*);

#endif
