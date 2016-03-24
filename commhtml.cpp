#include "commhtml.h"

extern SOCKET workingSocket;
extern int ComPort;
int HTML=0;
struct HTMLControls ControlVariable;

char* ReadJavaScriptFile(const char* filename)
{
  FILE* jsfile=fopen(filename,"rb");
  fseek(jsfile,0,SEEK_END);
  long jslength=ftell(jsfile);
  fseek(jsfile,0,SEEK_SET);
  char* jstxt=new char[jslength+1];
  fread(jstxt,1,jslength,jsfile);
  fclose(jsfile);
  jstxt[jslength]=0;
  return jstxt;
}

char* ReadHTMLFile(const char* filename,int& ftLength)
{
  FILE* htmlfile=fopen(filename,"rb");
  fseek(htmlfile,0,SEEK_END);
  long htmllength=ftell(htmlfile);
  fseek(htmlfile,0,SEEK_SET);
  char* htmltxt=new char[htmllength+1];
  fread(htmltxt,1,htmllength,htmlfile);
  fclose(htmlfile);
  htmltxt[htmllength]=0;
  int i=0;
  while(i<htmllength)
  {
    if(!compStr(htmltxt+i,"</script>"))
    {
      printf("Match found at %d\n",i);
      ftLength=i;
    }
    i++;
  }
  printf("%d %s\n",ftLength,htmltxt+ftLength);
  return htmltxt;
}

void sendSimpleHTML()
{
  char HTMLString0[]="HTTP/1.1 200 OK";
  char HTMLString1[]="Content-Type: text/html";
  char HTMLStringa[100];
  int finalTags;
  char* HTMLString2=ReadHTMLFile("demo.html",finalTags);
  char* HTMLString4=ReadJavaScriptFile("demo.js");
  char HTMLEOL[2]={0x0d,0x0a};
  int workingLength=strlen(HTMLString2)+strlen(HTMLString4);
  sprintf(HTMLStringa,"Content-Length: %d",workingLength);
  printf("Sending %d byte response\n",workingLength);
  printf("%s\n%d\n",HTMLString4,strlen(HTMLString4));
  fflush(stdout);
  send(workingSocket,HTMLString0,strlen(HTMLString0),NULL);
  send(workingSocket,HTMLEOL,2,NULL);
  send(workingSocket,HTMLString1,strlen(HTMLString1),NULL);
  send(workingSocket,HTMLEOL,2,NULL);
  send(workingSocket,HTMLStringa,strlen(HTMLStringa),NULL);
  send(workingSocket,HTMLEOL,2,NULL);
  send(workingSocket,HTMLEOL,2,NULL);
  send(workingSocket,HTMLString2,finalTags,NULL);
  send(workingSocket,HTMLString4,strlen(HTMLString4),NULL);
  send(workingSocket,HTMLString2+finalTags,strlen(HTMLString2)-finalTags,NULL);
  closesocket(workingSocket);
  delete HTMLString4;
  delete HTMLString2;
}

void HTMLOpenPort(int pn)
{
  char HTMLString0[]="HTTP/1.1 200 OK";
  char HTMLString1[]="Content-Type: text/html";
  char HTMLStringa[100];
  char HTMLString2[300];
  sprintf(HTMLString2,"<html><head><title>Port Opened</title></head><body><h1>Com Port Opened</h1><a href=\"127.0.0.1:56267\">Return</a><script>localStorage.setItem(\"Port\",%d)</script></body></html>",pn);
  char HTMLString3[]="<html><head><title>Port Failed</title></head><body><h1>Failed to open COM port</h1>Ensure the LaunchPad is connected properly.</p><a href=\"127.0.0.1:56267\">Return</a></body></html>";
  char HTMLEOL[2]={0x0d,0x0a};

  ComPort=EstablishComPort(NULL);
  if(ComPort)
  {
    int workingLength=strlen(HTMLString2);
    sprintf(HTMLStringa,"Content-Length: %d",workingLength);
    printf("Sending %d byte response\n",workingLength);
    printf("%s\n%d\n",HTMLString2,strlen(HTMLString2));
    fflush(stdout);
    send(workingSocket,HTMLString0,strlen(HTMLString0),NULL);
    send(workingSocket,HTMLEOL,2,NULL);
    send(workingSocket,HTMLString1,strlen(HTMLString1),NULL);
    send(workingSocket,HTMLEOL,2,NULL);
    send(workingSocket,HTMLStringa,strlen(HTMLStringa),NULL);
    send(workingSocket,HTMLEOL,2,NULL);
    send(workingSocket,HTMLEOL,2,NULL);
    send(workingSocket,HTMLString2,strlen(HTMLString2),NULL);
    closesocket(workingSocket);
  }
  else
  {
    int workingLength=strlen(HTMLString3);
    sprintf(HTMLStringa,"Content-Length: %d",workingLength);
    printf("Sending %d byte response\n",workingLength);
    printf("%s\n%d\n",HTMLString3,strlen(HTMLString3));
    fflush(stdout);
    send(workingSocket,HTMLString0,strlen(HTMLString0),NULL);
    send(workingSocket,HTMLEOL,2,NULL);
    send(workingSocket,HTMLString1,strlen(HTMLString1),NULL);
    send(workingSocket,HTMLEOL,2,NULL);
    send(workingSocket,HTMLStringa,strlen(HTMLStringa),NULL);
    send(workingSocket,HTMLEOL,2,NULL);
    send(workingSocket,HTMLEOL,2,NULL);
    send(workingSocket,HTMLString3,strlen(HTMLString3),NULL);
    closesocket(workingSocket);
  }
}

void sendHTMLGoodbye()
{
  char HTMLString0[]="HTTP/1.1 200 OK";
  char HTMLString1[]="Content-Type: text/html";
  char HTMLStringa[100];
  char HTMLString2[]="<html><head><title>Terminating Server</title></head><body><h1>Goodbye</h1></body></html>";
  char HTMLEOL[2]={0x0d,0x0a};
  int workingLength=strlen(HTMLString2);
  sprintf(HTMLStringa,"Content-Length: %d",workingLength);
  printf("Sending %d byte response\n",workingLength);
  printf("%s\n%d\n",HTMLString2,strlen(HTMLString2));
  fflush(stdout);
  send(workingSocket,HTMLString0,strlen(HTMLString0),NULL);
  send(workingSocket,HTMLEOL,2,NULL);
  send(workingSocket,HTMLString1,strlen(HTMLString1),NULL);
  send(workingSocket,HTMLEOL,2,NULL);
  send(workingSocket,HTMLStringa,strlen(HTMLStringa),NULL);
  send(workingSocket,HTMLEOL,2,NULL);
  send(workingSocket,HTMLEOL,2,NULL);
  send(workingSocket,HTMLString2,strlen(HTMLString2),NULL);
  closesocket(workingSocket);
}  
  

void HTMLPortExists()
{
  char HTMLString0[]="HTTP/1.1 200 OK";
  char HTMLString1[]="Content-Type: text/html";
  char HTMLStringa[100];
  char HTMLString2[]="<html><head><title>Port Exists</title></head><body><h1>Port already open</h1><a href=\"127.0.0.1:56267\">Return</a></body></html>";
  char HTMLEOL[2]={0x0d,0x0a};
  int workingLength=strlen(HTMLString2);
  sprintf(HTMLStringa,"Content-Length: %d",workingLength);
  printf("Sending %d byte response\n",workingLength);
  printf("%s\n%d\n",HTMLString2,strlen(HTMLString2));
  fflush(stdout);
  send(workingSocket,HTMLString0,strlen(HTMLString0),NULL);
  send(workingSocket,HTMLEOL,2,NULL);
  send(workingSocket,HTMLString1,strlen(HTMLString1),NULL);
  send(workingSocket,HTMLEOL,2,NULL);
  send(workingSocket,HTMLStringa,strlen(HTMLStringa),NULL);
  send(workingSocket,HTMLEOL,2,NULL);
  send(workingSocket,HTMLEOL,2,NULL);
  send(workingSocket,HTMLString2,strlen(HTMLString2),NULL);
  closesocket(workingSocket);
}  

void HTMLShowMotorSpeed(const char* ms)
{
  char HTMLString0[]="HTTP/1.1 200 OK";
  char HTMLString1[]="Content-Type: text/html";
  char HTMLStringa[100];
  char HTMLString2[300];
  char HTMLEOL[2]={0x0d,0x0a};
  sprintf(HTMLString2,"<html><head><title>Motor Speed</title></head><body><h1>Motor Speed Reset</h1>Current Motor Speed is %s</p><a href=\"127.0.0.1:56267\">Return</a><script>localStorage.setItem(\"Motor\",%s)</script></body></html>",ms,ms);
  int workingLength=strlen(HTMLString2);
  sprintf(HTMLStringa,"Content-Length: %d",workingLength);
  printf("Sending %d byte response\n",workingLength);
  printf("%s\n%d\n",HTMLString2,strlen(HTMLString2));
  fflush(stdout);
  send(workingSocket,HTMLString0,strlen(HTMLString0),NULL);
  send(workingSocket,HTMLEOL,2,NULL);
  send(workingSocket,HTMLString1,strlen(HTMLString1),NULL);
  send(workingSocket,HTMLEOL,2,NULL);
  send(workingSocket,HTMLStringa,strlen(HTMLStringa),NULL);
  send(workingSocket,HTMLEOL,2,NULL);
  send(workingSocket,HTMLEOL,2,NULL);
  send(workingSocket,HTMLString2,strlen(HTMLString2),NULL);
  closesocket(workingSocket);
}  

void HTMLMotorFailure()
{
  char HTMLString0[]="HTTP/1.1 200 OK";
  char HTMLString1[]="Content-Type: text/html";
  char HTMLStringa[100];
  char HTMLString2[]="<html><head><title>Motor Failure</title></head><body><h1>Failed to set Motor Speed</h1>Check that value is in range [8,F]<a href=\"127.0.0.1:56267\">Return</a></body></html>";
  char HTMLEOL[2]={0x0d,0x0a};
  int workingLength=strlen(HTMLString2);
  sprintf(HTMLStringa,"Content-Length: %d",workingLength);
  printf("Sending %d byte response\n",workingLength);
  printf("%s\n%d\n",HTMLString2,strlen(HTMLString2));
  fflush(stdout);
  send(workingSocket,HTMLString0,strlen(HTMLString0),NULL);
  send(workingSocket,HTMLEOL,2,NULL);
  send(workingSocket,HTMLString1,strlen(HTMLString1),NULL);
  send(workingSocket,HTMLEOL,2,NULL);
  send(workingSocket,HTMLStringa,strlen(HTMLStringa),NULL);
  send(workingSocket,HTMLEOL,2,NULL);
  send(workingSocket,HTMLEOL,2,NULL);
  send(workingSocket,HTMLString2,strlen(HTMLString2),NULL);
  closesocket(workingSocket);
}  

int ScanToEndOfHeader(const char* htxt,int rl,int& cl)
{
  int indx=0;
  int rval=-1;
  while(indx<rl && rval==-1)
  {
    if(!compStr(htxt+indx,"Content-Length"))
    {
      printf("%s\n",htxt+indx);
      if(!cl)
      {
	while(htxt[indx++]!=':');
	indx+=1;
	while(htxt[indx]==0x20)indx++;
	while(htxt[indx]>='0' && htxt[indx]<='9')
	{
	  cl=cl*10+(htxt[indx++]-'0');
	  printf("%d %c %x %d\n",indx,htxt[indx-1],htxt[indx-1],cl);
	}
	printf("%d\n",cl);
      }
    }
    else
    {
      if(indx>=3)
      {
	if(htxt[indx-3]==0x0d && htxt[indx-2]==0x0a && htxt[indx-1]==0x0d && htxt[indx]==0x0a)
	  rval=indx-3;
      }
    }
    indx+=1;
  }
  return rval;
}

MsgEnum ParseGetOptions(const char* optlist)
{
  int indx=5;
  printf("\n%30s\n",optlist+indx);
  ControlVariable.Port=-1;
  ControlVariable.MotorSpeed=-1;
  ControlVariable.Terminate=0;
  while(optlist[indx]!=0x20)
  {
    if(optlist[indx]=='&' || optlist[indx]=='?')
      indx+=1;
    if(!compStr(optlist+indx,"Port"))
    {
      printf("Checking Port ID\n");
      while(optlist[indx++]!='=');
      if(optlist[indx]>'0' && optlist[indx]<='9')
      {
	ControlVariable.Port=optlist[indx]-'0';
	printf("Value %d found\n",ControlVariable.Port);
      }
      fflush(stdout);
    }
    else if(!compStr(optlist+indx,"Motor"))
    {
      printf("Getting motor speed input\n");
      while(optlist[indx++]!='=');
      if(optlist[indx]>='0' && optlist[indx]<='9')
	ControlVariable.MotorSpeed=optlist[indx]-'0';
      else if (optlist[indx]>='A' && optlist[indx]<='F')
	ControlVariable.MotorSpeed=optlist[indx]-'A'+10;
      else if (optlist[indx]>='a' && optlist[indx]<='f')
	ControlVariable.MotorSpeed=optlist[indx]-'a'+10;
    }
    else if(!compStr(optlist+indx,"Terminate"))
    {
      ControlVariable.Terminate=1;
    }
    while(optlist[indx]!=0x20 && optlist[indx]!='&')
    {
      printf("%d %c\n",indx,optlist[indx]);
      indx++;
    }
  }
  char inVal[60];
  while(readIndefiniteFromSocket(inVal,60)==60)
    printf("%s",inVal);
  printf("%s",inVal);
  if(ControlVariable.Port>0)
    return OpenCOMPort;
  else if (ControlVariable.MotorSpeed>0)
    return MotorCommand;
  else if (ControlVariable.Terminate)
    return Terminate;
  else
    return Unrecognized;
}

MsgEnum ParsePostOptions(const char* optlist)
{
  char inVal[150];
  int contentLength=0;
  int readLength;
  while((readLength=readIndefiniteFromSocket(inVal,150))==150)
  {
    int tagloc=ScanToEndOfHeader(inVal,readLength,contentLength);
    printf("%s",inVal);
    if(tagloc>=0)
      printf("Header ends at %d\n",tagloc);
    if(contentLength)
      printf("Content length is %d\n",contentLength);
  }
  int tagloc=ScanToEndOfHeader(inVal,readLength,contentLength);
  printf("%s",inVal);
  if(tagloc>=0)
    printf("Header ends at %d\n",tagloc);
  if(contentLength)
    printf("Content length is %d\n",contentLength);
  int indx=tagloc+4;
  ControlVariable.Port=-1;
  ControlVariable.MotorSpeed=-1;
  ControlVariable.Terminate=0;
  while(indx<readLength)
  {
    if(inVal[indx]=='&' || inVal[indx]=='?')
      indx+=1;
    if(!compStr(inVal+indx,"Port"))
    {
      printf("Checking Port ID\n");
      while(inVal[indx++]!='=');
      if(inVal[indx]>'0' && inVal[indx]<='9')
      {
	ControlVariable.Port=inVal[indx]-'0';
	printf("Value %d found\n",ControlVariable.Port);
      }
      else if(inVal[indx]=='0')
      {
        ControlVariable.Port=-5;
        printf("Using default value of port\n");
      }
      fflush(stdout);
    }
    else if(!compStr(inVal+indx,"Motor"))
    {
      printf("Getting motor speed input\n");
      while(inVal[indx++]!='=');
      if(inVal[indx]>='0' && inVal[indx]<='9')
	ControlVariable.MotorSpeed=inVal[indx]-'0';
      else if (inVal[indx]>='A' && inVal[indx]<='F')
	ControlVariable.MotorSpeed=inVal[indx]-'A'+10;
      else if (inVal[indx]>='a' && inVal[indx]<='f')
	ControlVariable.MotorSpeed=inVal[indx]-'a'+10;
    }
    else if(!compStr(inVal+indx,"Terminate"))
    {
      ControlVariable.Terminate=1;
    }
    while(indx<readLength && inVal[indx]!='&')
    {
      printf("%d %c\n",indx,inVal[indx]);
      indx++;
    }
  }
  if(ControlVariable.Port>0  || ControlVariable.Port==-5)
    return OpenCOMPort;
  else if (ControlVariable.MotorSpeed>0)
    return MotorCommand;
  else if (ControlVariable.Terminate)
    return Terminate;
  else
    return Unrecognized;
}
