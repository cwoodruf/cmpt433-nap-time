#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "pushbuttondrv.h"


#define FIFO_NAME "/tmp/madplayFIFO"
#define FALSE 0
#define TRUE 1
#define DEBUG 1

int pipe1;

void pause1()
{
  pipe1 = open(FIFO_NAME, O_WRONLY);     
  write(pipe1, "p", 1);  
  if (DEBUG) printf("pause\n");
  //sleep(10);
  close(pipe1);
}

void resume()
{
  pipe1 = open(FIFO_NAME, O_WRONLY);    
  write(pipe1, "o", 1);
  if (DEBUG) printf("resume\n");
  //sleep(10);
  close(pipe1);

}

void stop()
{
   pipe1 = open(FIFO_NAME, O_WRONLY);    
   write(pipe1, "s", 1);
   if (DEBUG) printf("stop\n");
   //sleep(10);
   close(pipe1);

}

void forward()
{
   pipe1 = open(FIFO_NAME, O_WRONLY);    
   write(pipe1, "f", 1);
   if (DEBUG) printf("forward\n");
   //sleep(20);
   close(pipe1); 
}

void backward()
{
   pipe1 = open(FIFO_NAME, O_WRONLY);    
   write(pipe1, "b", 1);
   if (DEBUG) printf("backward\n");
   //sleep(20);
   close(pipe1);
}

void increasevolume()
{
  pipe1 = open(FIFO_NAME, O_WRONLY);
  write(pipe1, "+", 1);
  if (DEBUG) printf("increase volume\n");
  close(pipe1);
}

void reducevolume()
{
  pipe1 = open(FIFO_NAME, O_WRONLY);
  write(pipe1, "-", 1);
  if (DEBUG) printf("reduce volume\n");
  close(pipe1);
}

void quit()
{
  pipe1 = open(FIFO_NAME, O_WRONLY);    
  write(pipe1, "q", 1);
  if (DEBUG) printf("quit\n");
  close(pipe1);
}

int main()
{
  int buttonPressed;
  int paused = FALSE;
  PUSHBUTTONDrv_init();

  while(1){
    //if (DEBUG) printf("converted pressed code is %d\n", PUSHBUTTONDrv_detect());
    
    buttonPressed = PUSHBUTTONDrv_detect();
    if (DEBUG) printf("button pressed is %d\n", buttonPressed);
    if (!paused){
      if (buttonPressed == 9)
	quit();
      if (buttonPressed & RIGHT_BUTTON_REP)
        forward();
      if (buttonPressed & LEFT_BUTTON_REP)
        backward();
      if (buttonPressed == 6){
        pause1();
        paused = TRUE;
      }else if (buttonPressed & DOWN_BUTTON_REP)
        reducevolume();
      else if (buttonPressed & UP_BUTTON_REP)
        increasevolume();
      
    }else{
      if (buttonPressed == 6){
	resume();
	paused = FALSE;
      }
      //if (buttonPressed == 9)
	//quit();
      //if (buttonPressed & DOWN_BUTTON_REP)
      //  reducevolume();
      //else if (buttonPressed & UP_BUTTON_REP)
      //  increasevolume();
      
    }

  }
  PUSHBUTTONDrv_cleanup();
  

}
