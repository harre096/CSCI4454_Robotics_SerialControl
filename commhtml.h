#include <cstdio>
#include "commutil.h"

#ifndef COMMHTML_H_INCLUDED
#define COMMHTML_H_INCLUDED


struct HTMLControls {
  int Port;
  int MotorSpeed;
  int Terminate;
};

char* ReadJavaScriptFile(const char*);
char* ReadHTMLFile(const char*,int&);
void sendSimpleHTML(void);
void HTMLOpenPort(int);
void sendHTMLGoodbye(void);
void HTMLPortExists(void);
void HTMLShowMotorSpeed(const char*);
void HTMLMotorFailure(void);
int ScanToEndOfHeader(const char*,int,int&);
MsgEnum ParseGetOptions(const char*);
MsgEnum ParsePostOptions(const char*);

#endif
