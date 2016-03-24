#include "commutil.h"

SOCKET roboSock;
SOCKET workingSocket;
struct sockaddr_in roboServer;
fd_set socketSet;
int ComPort=NULL;

int readIndefiniteFromSocket(char* msg,int bufferLength)
{
  int br=recv(workingSocket,msg,bufferLength,MSG_PEEK);
  recv(workingSocket,msg,br,MSG_WAITALL);
  printf("Read %d bytes\n",br);
  for(int i=0;i<br;++i)
    printf("%02x%s",(unsigned char)msg[i],(i==(br-1))?"\n":" ");
  fflush(stdout);
  return br;
}

void openCOMPort(const char* pn)
{
  ComPort=EstablishComPort(pn);
  if(ComPort)
    send(workingSocket,"OK",2,NULL);
  else
    send(workingSocket,"FL",2,NULL);
}

int compStr(const char* s1,const char* s2)
{
  /* C (and C++) have the strcmp function which almost does this.  But
  for this particular application, s1 may be much longer than s2, and we
  only want to match the first part.  This is asymmetric- s1 can be longer than
  s2, but not shorter.*/
  int i=0;
  while(s1[i]!=0 && s2[i]!=0)
  {
    if(s1[i]!=s2[i])
    {
      return i+1;
    }
    i++;
  }
  if(s1[i]!=0 || s2[i]==s1[i])
    return 0;
  else
    return i;
}
