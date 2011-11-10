/* 
 * napping.c
 * send a broadcast out to other nap nodes and see who responds
 * will wait a specific amount of time for responses before quitting
 *
 * adapted from
 * sender.c -- multicasts "hello, world!" to a multicast group once a second
 * http://ntrg.cs.tcd.ie/undergrad/4ba2/multicast/antony/example.html
 * Antony Courtney,        25/11/94
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include "napmulticast.h"

/* exit after a certain amount of time */
void sig_alarm(int i) 
{
     exit(0);
}

int main(int argc, char *argv[])
{
     struct sockaddr_in addr,laddr;
     socklen_t laddr_len;
     int fd, listen_fd, nbytes;
     /* struct ip_mreq mreq; */
     char message[NAPMSGLEN]="peer", responsemsg[NAPMSGLEN]="";
     char *napgroup = NAP_GROUP;
     char *peerdir = NAP_PEER_DIR;
     struct stat st; /* for checking if peer base directory exists */
     char dir[UNIX_PATH_MAX];
     int napport = NAP_PORT;
     int alarmwait = NAP_WAIT;
     int c;

     signal(SIGALRM,sig_alarm);

     while ((c = getopt(argc,argv,"d:g:p:t:h")) > 0) {
          switch(c) {
          case 'h': printf("%s -g{mcastgroup} -p{port} -t{timeout} -d{peerdir} [-h]\n"
                           "\tListens for nap hosts on a local multicast group.\n"
                           "\t-g{mcastgroup} multicast group (%s)\n"
                           "\t-p{port} port to listen on (%d)\n"
                           "\t-t{timeout} wait for this many seconds for responses (%d)\n"
                           "\t-h this help.\n",
                           argv[0],napgroup,napport,alarmwait
                    ); 
          return 0;
          case 'g': napgroup = optarg; 
          break;
          case 'p': napport = atoi(optarg); 
                    if (napport < PORTSTART || napport > PORTEND) {
                         fprintf(stderr,"port out of range (%d to %d)\n", PORTSTART,PORTEND);
                         return 1;
                    }
          break;
          case 'd': peerdir = optarg;
                    if (stat(peerdir,&st) < 0) {
                          perror("peerdir stat");
                          return 1;
                    }
                    if (!S_ISDIR(st.st_mode)) {
                          fprintf(stderr,"peer dir %s is not a directory!\n",peerdir);
                          return 1;
                    }
          break;
          case 't': alarmwait = atoi(optarg); 
                    if (alarmwait < 0) {
                          fprintf(stderr,"timeout should be more than 0!\n");
                          return 1;
                    }
          break;
          default: printf("unknown option try -h for help\n"); return 1;
          }
     }
     /* create what looks like an ordinary UDP socket */
     if ((fd=socket(AF_INET,SOCK_DGRAM,0)) < 0) {
          perror("socket");
          return 1;
     }
     /* listening socket for response from other nap nodes */
     if ((listen_fd=socket(AF_INET,SOCK_DGRAM,0)) < 0) {
          perror("listen socket");
          return 1;
     }
     /* set up listen address */
     memset(&laddr,0,sizeof(laddr));
     laddr.sin_family=AF_INET;
     laddr.sin_addr.s_addr=htonl(INADDR_ANY); 
     laddr.sin_port=htons(napport+1);

     /* bind to receive address */
     if (bind(listen_fd,(struct sockaddr *) &laddr,sizeof(laddr)) < 0) {
          perror("bind");
          return 1;
     }

     /* set up destination address */
     memset(&addr,0,sizeof(addr));
     addr.sin_family=AF_INET;
     addr.sin_addr.s_addr=inet_addr(napgroup);
     addr.sin_port=htons(napport);
     
     /* now just sendto() our destination! */
     if (sendto(fd,message,strlen(message),0,(struct sockaddr *) &addr, sizeof(addr)) < 0) {
               perror("sendto");
               return 1;
     }
     nbytes = 0;

     alarm(alarmwait);
     do {
          if ((nbytes=recvfrom(listen_fd,(void *)responsemsg,NAPMSGLEN,0,
                               (struct sockaddr *)&laddr,&laddr_len)) < 0) {
                 perror("recvfrom");
                 return 1;
          }
          responsemsg[nbytes] = 0;
          if (strcmp(responsemsg,NAP_ACK)) {
               fprintf(stderr,"unexpected message %s from %s - aborting\n",
                       responsemsg,inet_ntoa(laddr.sin_addr));
               continue;
          }
          snprintf(dir,UNIX_PATH_MAX,"%s/%s",peerdir,inet_ntoa(laddr.sin_addr));
          printf("got %s ",responsemsg);
          if (stat(dir,&st) == 0) {
               printf("%s already exists in file system\n",dir);
               if (!S_ISDIR(st.st_mode)) fprintf(stderr,"ERROR: %s is not a directory!\n",dir);
               continue;
          }
          printf("making directory %s\n",dir);
          if (mkdir(dir,0777) < 0) {
                perror("mkdir");
          }
     } while (nbytes > 0);
     return 0;
}

