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

// check whether the parameter is a mp3 file
int isMusicfile(char *file){
  int size = strlen(file);
  if (file[size - 4] == '.' && file[size - 3] == 'm'
    && file[size - 2] == 'p' && file[size -1] == '3')
    return TRUE;
  else
    return FALSE;  
}

// return sha1sum of given file
char* getSha1Sum(char *file){
  char *ret;
  char buf[BUFSIZ], cmd[100];
  FILE *ptr;

  memset(cmd, '\0', sizeof(cmd));

  strcat(cmd, "sha1sum ");
  strcat(cmd, file);  
  
  if ((ptr = popen(cmd, "r")) != NULL){
    while (fgets(buf, BUFSIZ, ptr) != NULL)      
      ret = strtok(buf, " ");    
  }
  return ret;  
}

// return file size of a given file
long getFilesize(char *file){
  struct stat st;
  stat(file, &st);
  return st.st_size;
}


int main()
{
  size_t len = 100;
  char *cmd = "fdisk -l";
  char buf[BUFSIZ], hostname[len], mountpt[len], musiclist[len], locallist[len], filename[len], strItem[200]; 
  struct DeviceInfo devInfo;  
  FILE *ptr, *fs;  
  int linecount, prevlinecount = 0, bUSBInsert = FALSE, bUSBRemove = FALSE;
  struct stat st;
  DIR *usbDir;
  struct dirent *entry = NULL;

  // create a directory that is unique to every board
  strcpy(mountpt, "/web/udisk");
  gethostname(hostname, len);  
  strcat(mountpt, hostname); 

  // check whether mount folder already exists, if not, create it.
  if (stat(mountpt, &st) != 0)
    mkdir(mountpt, 0777);

  strcpy(musiclist, "/web/musiclist/");
  strcpy(locallist, musiclist);
  strcat(locallist, "locallist");
  
  // check whether music list folder already exists, if not, create it.
  if (stat(musiclist, &st) != 0)
    mkdir(musiclist, 0777);

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
      }
      
      if(bUSBInsert){
        devInfo = getDevInfo(buf);	
	mount(devInfo.devType, mountpt, "vfat", MS_MGC_VAL | MS_NOSUID, "");
	fs = fopen(locallist, "w");
        if (fs == NULL){
	  perror("File local list can not be opened for writing\n");
  	}else{
          usbDir = opendir(mountpt);
	  // loop through every file in the usb folder.
	  // a music file will be written to music list file along with its sha1sum, size and path.
	  while((entry = readdir(usbDir))){
	    memset(filename, '\0', sizeof(filename));
            if (isMusicfile(entry->d_name)){              
	      strcat(filename, mountpt);
	      strcat(filename, "/");
	      strcat(filename, entry->d_name);              
	      
	      sprintf(strItem, "%s\t%ld\t%s\n", getSha1Sum(filename), getFilesize(filename), filename);
	      printf("%s\n", strItem);
	      fwrite(strItem, strlen(strItem), 1, fs);
 	    }
          }
          closedir(usbDir);
        }
	fclose(fs);        
      }else if(bUSBRemove) { 
	// if a usb flash drive is just removed, unmount the usb and clear the music list file.       
 	umount2(mountpt, MNT_FORCE);
	fs = fopen(locallist, "w");
	fclose(fs);	
      }

      prevlinecount = linecount;                      
      (void) pclose(ptr);
      sleep(10);
    }      
  }
  return 0;
}
