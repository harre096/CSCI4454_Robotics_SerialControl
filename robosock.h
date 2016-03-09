#include <cstdio>
#include <cstdlib>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cmath>
#include "commbase.h"
#include "commutil.h"
#include "commhtml.h"

#ifndef ROBOSOCK_H_INCLUDED
#define ROBOSOCK_H_INCLUDED

int initializeSocket(void);
void releaseSocket(void);
int socketRequested(void);
int connectSocket(void);

#endif
