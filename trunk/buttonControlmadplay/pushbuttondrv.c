#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include "pushbuttondrv.h"

int fdButton;
struct timeval tv;
fd_set readfds;

int PUSHBUTTONDrv_init()
{
  fdButton = open("/dev/IRQ-Test", O_RDONLY);
  if (fdButton < 0) {
    perror("open device EmbedSky-PushButton");
    return 1;
  }

  tv.tv_sec = 30000;
  tv.tv_usec = 500000;

  FD_ZERO(&readfds);
  FD_SET(fdButton, &readfds);

  return 0;
}

void PUSHBUTTONDrv_cleanup()
{
  close(fdButton);
}

int PUSHBUTTONDrv_detect()
{
  int result, res = 0;
  char nread[5];
  result = select(fdButton+1, &readfds, NULL, NULL, &tv);
  if (FD_ISSET(fdButton, &readfds)){
    read(fdButton, (void *)&nread, 4);
    nread[4] = '\0';
    if (nread[UP_BUTTON] == '1')
      res += UP_BUTTON_REP;
    if (nread[DOWN_BUTTON] == '1')
      res += DOWN_BUTTON_REP;
    if (nread[LEFT_BUTTON] == '1')
      res += LEFT_BUTTON_REP;
    if (nread[RIGHT_BUTTON] == '1')
      res += RIGHT_BUTTON_REP;    
  }else
    res = TIME_OUT;
  return res;  
}

/*int main()
{
  int res;  

  PUSHBUTTONDrv_init();
  while(1){
    printf("converted pressed code is %d\n", PUSHBUTTONDrv_detect());    
  }
  PUSHBUTTONDrv_cleanup(); 
}*/


