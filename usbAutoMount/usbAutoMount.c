#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/mount.h>
#include <sys/stat.h>

#define TRUE 1
#define FALSE 0

struct DeviceInfo{
  char devType[10];
  char fsType[10];
};

struct DeviceInfo getDevInfo(char *buf){
  struct DeviceInfo ret;
  char *dType;
  
  dType = strtok(buf, " ");  
  strcpy(ret.devType, dType);
  
  return ret;
}


int main()
{
  size_t len = 50;
  char *cmd = "fdisk -l";
  char buf[BUFSIZ], hostname[len], mountpt[len];; 
  struct DeviceInfo devInfo;  
  FILE *ptr;  
  int linecount, prevlinecount = 0, bUSBInsert = FALSE, bUSBRemove = FALSE;
  struct stat st;

  // create a directory that is unique to every board
  strcpy(mountpt, "/mnt/udisk");
  gethostname(hostname, len);  
  strcat(mountpt, hostname); 

  // check whether folder already exists, if not, create it.
  if (stat(mountpt, &st) != 0)
    mkdir(mountpt, 0777);

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
	mount(devInfo.devType, mountpt, "vfat", MS_MGC_VAL | MS_NOSUID, "");        
      }else if(bUSBRemove) {        
 	umount2(mountpt, MNT_FORCE);	
      }

      prevlinecount = linecount;                      
      (void) pclose(ptr);
      sleep(10);
    }      
  }
  return 0;
}
