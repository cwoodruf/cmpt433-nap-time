#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/mount.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>

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
  //getcwd(ret.fsType, sizeof(ret.fsType));
  //printf("Fs Type is %s\n", ret.fsType);  
  return ret;
}


int main()
{
  size_t len = 100;
  char *ip_last, *cmd = "fdisk -l";
  char buf[BUFSIZ], mountpt[len], mountStr[len], umountStr[len]; 
  struct DeviceInfo devInfo;  
  FILE *ptr, *fs;  
  int linecount, prevlinecount = 0, bUSBInsert = FALSE, bUSBRemove = FALSE;
  struct stat st;
    

  // create a directory that is unique to every board
  strcpy(mountpt, "/mnt/udisk");
  
  // check whether mount folder already exists, if not, create it.
  if (stat(mountpt, &st) != 0)
    mkdir(mountpt, 0777);

  

  while (TRUE){
    linecount = 0;
    if ((ptr = popen(cmd, "r")) != NULL){
      
      while (fgets(buf, BUFSIZ, ptr) != NULL)
        linecount ++;
      
      // check whether a usb key is just inserted or just removed.      
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
      }// end with if (linecount != prevlinecount)      
      
      if(bUSBInsert){
        devInfo = getDevInfo(buf);

	strcpy(mountStr, "mount ");
        strcat(mountStr, devInfo.devType);
        strcat(mountStr, " ");
 	strcat(mountStr, mountpt);

 	system(mountStr);
	strcpy(umountStr, "");

        //printf("Mount point is %s\n", mountpt);	
	//mount(devInfo.devType, mountpt, "vfat", MS_MGC_VAL | MS_NOSUID, "");
	       
      }else if(bUSBRemove) { 
	// if a usb flash drive is just removed, unmount the usb and clear the music list file.
	strcpy(umountStr, "umount ");
	strcat(umountStr, mountpt);
	strcat(umountStr, " ");
	system(umountStr);
	strcpy(mountStr, "");       
 	//umount2(mountpt, MNT_FORCE);
		
      } //end with if(bUSBRemove)

      prevlinecount = linecount;                      
      (void) pclose(ptr);
      sleep(10);
    }// end with if ((ptr = popen(cmd, "r")) != NULL)      
  }// end with while (TRUE)
  return 0;
}
