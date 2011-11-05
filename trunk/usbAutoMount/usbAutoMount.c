#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/mount.h>

#define TRUE 1
#define FALSE 0

struct DeviceInfo{
  char devType[10];
  char fsType[10];
};

struct DeviceInfo getDevInfo(char *buf){
  struct DeviceInfo ret;
  char *dType, *fType;
  int count = 0;
  dType = strtok(buf, " ");
  /*while (count < 6){
    fType = strtok(NULL, " ");
    count ++;
  }*/
  strcpy(ret.devType, dType);
  //strcpy(ret.fsType, fType);
  return ret;
}


int main()
{
  char *cmd = "fdisk -l";
  char buf[BUFSIZ]; 
  struct DeviceInfo devInfo;  
  FILE *ptr;  
  int linecount, prevlinecount = 0;
  int bUSBInsert = FALSE, bUSBRemove = FALSE;

  while (TRUE){
    linecount = 0;
    if ((ptr = popen(cmd, "r")) != NULL){
      
      while (fgets(buf, BUFSIZ, ptr) != NULL)
        linecount ++;
            
      if (linecount != prevlinecount){
        if (linecount){
          bUSBInsert = TRUE;
          bUSBRemove = FALSE;
        }else{
	  bUSBRemove = TRUE;
          bUSBInsert = FALSE;
        }
      }else{
        bUSBInsert = bUSBRemove = FALSE;
      }
      
      if(bUSBInsert){
        devInfo = getDevInfo(buf);	
	mount(devInfo.devType, "/mnt/udisk", "vfat", MS_MGC_VAL | MS_NOSUID, "");        
      }else if(bUSBRemove) {        
 	umount2("/mnt/udisk", MNT_FORCE);	
      }

      prevlinecount = linecount;                      
      (void) pclose(ptr);
      sleep(10);
    }      
  }
  return 0;
}
