#include "robosock.h"

extern SOCKET roboSock;
extern SOCKET workingSocket;
extern struct sockaddr_in roboServer;
extern fd_set socketSet;
extern int ComPort;
static int telnet=0;
extern int HTML;
extern struct HTMLControls ControlVariable;


int CreateSocket(void)
{
 roboSock=socket(AF_INET,SOCK_STREAM,0);
  if(roboSock<0)
  {
    fprintf(stderr,"Unable to create socket.  Error code %d\n",errno);
    return -2;
  }
  return 0;
}

int BindSocketAndListen(void)
{
  const int RobotPort=56267;
  roboServer.sin_family=AF_INET;
  //  roboServer.sin_addr.s_addr=inet_addr("127.0.0.1");
  roboServer.sin_addr.s_addr=INADDR_ANY;
  
  roboServer.sin_port=htons(RobotPort);
  int error=bind(roboSock,(SOCKADDR*)&roboServer,sizeof(roboServer));
  if(error)
  {
    fprintf(stderr,"Unable to bind socket.  Error code %d\n",error);
    closesocket(roboSock);
    return -1;
  }
  if(listen(roboSock,10)==SOCKET_ERROR)
  {
    fprintf(stderr,"Unable to listen to socket.  Error code %d\n",error);
    close(roboSock);
     return -2;
  }
  return 0;
}

double translateToDouble(const char* bytes)
{
  double q1;
  for(int i=0;i<8;++i)
  {
    *((char*)(&q1)+i)=bytes[7-i];
  }
  return q1;
}

void translateFromDouble(double d,char* bytes)
{
  for(int i=0;i<8;++i)
  {
    bytes[7-i]=*((char*)(&d)+i);
  }
}

int translateToInt(const char* bytes)
{
  int vtr;
  for(int i=0;i<4;++i)
  {
    *((char*)(&vtr)+i)=bytes[3-i];
  }
  return vtr;
}

void translateFromInt(int v,char* bytes)
{
  for(int i=0;i<4;++i)
  {
    bytes[3-i]=*((char*)(&v)+i);
  }
}

int AcceptSocket(sockaddr_in& cSA)
{
  int cSAsize=sizeof(cSA);
  workingSocket=accept(roboSock,(SOCKADDR*)&cSA,(socklen_t*)&cSAsize);
  if(workingSocket==INVALID_SOCKET)
  {
    fprintf(stderr,"Invalid socket request.  Error is %d\n",errno);
    closesocket(roboSock);
    return -1;
  }
}

void welcomePort(void)
{
  send(workingSocket,"Hello",5,NULL);
}

void sendAckToPort(void)
{
  send(workingSocket,"?",1,NULL);
}

void sendIntToPort(int its)
{
  char inVal[4];
  translateFromInt(its,inVal);
  send(workingSocket,inVal,4,NULL);
}

  
int getIntFromPort(void)
{
  char inVal[30];

  printf("Reading number from socket\n");fflush(stdout);
  int nbr=readIndefiniteFromSocket(inVal);
  printf("Received %d bytes\n");
  for(int i=0;i<nbr;++i)
    printf("%0x02x ",inVal[i]);
  printf("\n");fflush(stdout);
  return translateToInt(inVal);
}

int getTelnetInt(void)
{
  char inVal[30];
  int indx=0;
  while(indx==0)
  {
    readIndefiniteFromSocket(inVal);
    indx=0;
    while(inVal[indx]!=0x0d && inVal[indx]!=0x0a)
      indx+=1;
    inVal[indx]=0;
  }
  int rval=strtol(inVal,NULL,16);
  printf("Returning %d (%x)\n",rval,rval);
  return rval;
}
  
void SendPortMessage(int ml,const char* msg)
{
  int nbs=WriteComPort(ComPort,ml,msg);
  sendIntToPort(nbs);
}

void RecvPortMessage(int bytesExpected)
{
  unsigned char msgBuff[100];
  int bytesRead=ReadComPort(ComPort,bytesExpected,msgBuff);
  send(workingSocket,(const char*)msgBuff,bytesRead,NULL);
}

void sendErrorToPort(void)
{
  send(workingSocket,"!",1,NULL);
}

void sendGoodbyeToPort(void)
{
  send(workingSocket,"Goodbye",7,NULL);
}

double getDoubleFromPort(void)
{
  char inVal[30];
  readIndefiniteFromSocket(inVal);
  double q=translateToDouble(inVal);
  return q;
}

void sendDoubleToPort(double d)
{
  char inVal[10];
  translateFromDouble(d,inVal);
  for(int i=0;i<8;++i)
    printf("%02x ",inVal[i]);
  printf("\n");
  send(workingSocket,inVal,8,NULL);
  printf("Done\n");
  fflush(stdout);
}


int isHTML(const char* msg)
{
  if(!compStr(msg,"GET"))
    return 1;
  else if(!compStr(msg,"POST"))
    return 2;
  else
    return 0;
}



MsgEnum getMessageFromPort(void)
{
  char inVal[60]={0x0d,0x0e,0x0a,0x0d,0x0b,0x0e,0x0e,0x0f};
  MsgEnum portMessage;
  int ml=readIndefiniteFromSocket(inVal,60);
  printf("%d byte message received\n",ml);
  fflush(stdout);
  if(ml==0)
    return Ignore;	
  if(ml<60)
  {
    if(ml<=2)
      ml=3;
    printf("Terminating string at byte %d (%d)\n",ml,sizeof(inVal));fflush(stdout);
    inVal[ml]=0;
    printf("%s\n",inVal);
    printf("%x\n",inVal[1]);
  }
  fflush(stdout);
  HTML=isHTML(inVal);
  printf("HTML received: %d\n",HTML);
  fflush(stdout);
  telnet=0;
  if(HTML)
  {
    printf("%s",inVal);
    printf("HTML %s detected.",(HTML==1)?"GET":"POST");
    fflush(stdout);
    if(HTML==1)
    {
      portMessage=ParseGetOptions(inVal);
    }
    else
    {
      portMessage=ParsePostOptions(inVal);
    }
  }
  else
  {
    printf("Switching on %x (%c)\n",inVal[0],inVal[0]);
    fflush(stdout);
    switch(inVal[0])
    {
    case 'x':
    case 'X':
      portMessage=Exit;
      break;
    case 't':
    case 'T':
      portMessage=Terminate;
      break;
    case 'P':
      telnet=1;
    case 'p':
      portMessage=OpenCOMPort;
      break;
    case 'M':
      telnet=1;
    case 'm':
      portMessage=MotorCommand;
      break;
    case 'C':
    case 'c':
      portMessage=Ignore;
      break;
    default:
      portMessage=Unrecognized;
    }
  }
  printf("%d %d\n",(int)portMessage,(int)OpenCOMPort);
  fflush(stdout);
  return portMessage;
}

int initializeSocket(void)
{
  if(CreateSocket()<0)
  {
    printf("Error in socket creation.\n");
    return 1;
  }
  if(BindSocketAndListen()<0)
  {
    printf("Error in socket binding\n");
    return 2;
  }
  return 0;
}

void releaseSocket(void)
{
  closesocket(roboSock);
}

int socketRequested(void)
{
  struct timeval socketTimeout={1,0};
  FD_ZERO(&socketSet);
  FD_SET((SOCKET)roboSock,&socketSet);
  printf("In socketRequested  roboSock is %d\n",roboSock);fflush(stdout);
  int selRet=select(roboSock+1,&socketSet,NULL,NULL,&socketTimeout);
  if(selRet==SOCKET_ERROR)
  {
    printf("Error in socket connection %d\n",errno);
    return -1;
  }
  else
    return selRet;
}

int connectSocket(void)
{
  sockaddr_in clientSockAddr;
  int terminateFlag=0;
  
  if(AcceptSocket(clientSockAddr)<0)
  {
    printf("Error in socket connection\n");
    return -1;
  }
  printf("Connection established to %s\n","hfhf");
  fflush(stdout);
  int connected=1;
  while(connected)
  {
    printf("Waiting for message\n");
    fflush(stdout);
    MsgEnum portMessage=getMessageFromPort();
    if(!HTML)
    {
      if(connected==1)
	welcomePort();
      connected=2;
    }
    printf("%d %d\n",(int)portMessage,(int)OpenCOMPort);
    fflush(stdout);
    switch(portMessage)
    {
    case Ignore:
      {
	printf("Handshake received\n");fflush(stdout);
	break;
      }
    case Exit:
      {
	printf("Closing connection\n");
	sendGoodbyeToPort();
	closesocket(workingSocket);
	connected=0;
	break;
      }
    case OpenCOMPort:
      {
	printf("Opening COM Port\n");
	fflush(stdout);
	if(!ComPort)
	{
	  printf("Port does not exist\n");fflush(stdout);
	  if(!HTML)
	    sendAckToPort();
	  int pt;
	  printf("%s\n",(telnet)?"TELNET":(HTML)?"HTML":"Script");fflush(stdout);
	  if(telnet)
	    pt=getTelnetInt();
	  else if(HTML)
	    pt=ControlVariable.Port;
	  else
	    pt=getIntFromPort();
	  printf("PortID %d (%x) received\n",pt,pt);
	  fflush(stdout);
	  if(!HTML)
	    openCOMPort(NULL);
	  else
	    HTMLOpenPort(NULL);
	}
	else
	{
	  if(!HTML)
	    send(workingSocket,"EXISTS",6,NULL);
	  else
	    HTMLPortExists();
	}
	fflush(stdout);
	break;
      }
    case MotorCommand:
      {
	if(!HTML)
	  sendAckToPort();
	int pt;
	if(telnet)
	  pt=getTelnetInt();
	else if(HTML)
	  pt=ControlVariable.MotorSpeed;
	else
	  pt=getIntFromPort();
	printf("New Motor speed setting %x\n",pt);
	unsigned char pw;
	if(pt>=0x0a)
	  pw=(pt&0x000F)+'A'-10;
	else if(pt==8 || pt==9)
	  pw=pt+'0';
	else
	  pw='8';
	printf("Writing %c to Motor\n",pw);
	WriteComPort(ComPort,1,(const char*)(&pw));
	char response[10];
	ReadComPort(ComPort,1,(unsigned char*)response);
	printf("First character back is %c\n",response[0]);fflush(stdout);
	if(response[0]!='X')
	{
          printf("Reading next 3 characters\n");fflush(stdout);
	  ReadComPort(ComPort,3,(unsigned char*)(response+1));
	  printf("Motor set to %s\n",response);
	  if(HTML)
	    HTMLShowMotorSpeed(response);
	  else
	    send(workingSocket,response,4,NULL);
	}
	else
	{
	  if(HTML)
	    HTMLMotorFailure();
	  else
	    send(workingSocket,response,1,NULL);
	}
	response[4]=0;
	printf("%s\n",response);
	break;
      }
    case Terminate:
      {
	printf("Closing connection and exiting\n");
	if(!HTML)
	  sendGoodbyeToPort();
	else
	  sendHTMLGoodbye();
	connected=0;
	terminateFlag=1;
	break;
      }
    case Unrecognized:
      {
	if(HTML)
	  sendSimpleHTML();
	else
	  sendErrorToPort();
	break;
      }
    default:
      sendErrorToPort();
    }
    if(HTML)
      connected=0;
    printf("Out of inner loop\n");
    fflush(stdout);
  }
  printf("Not connected\n");
  fflush(stdout);
  return terminateFlag;
}


