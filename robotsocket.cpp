#include "robosock.h"

int main(void)
{
  if(initializeSocket())
    exit(0);
  printf("Socket initialized\n");fflush(stdout);
  int terminateFlag=0;
  while(!terminateFlag)
  {
    int sR=socketRequested();
    if(sR>0)
    {
      printf("socketRequested returned %d\n",sR);fflush(stdout);
      terminateFlag=connectSocket();
    }
    else if(sR<0)
      terminateFlag=1;
  }
  releaseSocket();
  return EXIT_SUCCESS;
}
