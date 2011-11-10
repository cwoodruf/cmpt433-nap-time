/*
 * cmpt433 nap time group project
 * 
 * naplistener.c 
 * listen for local multicast packets from our local subnet
 * save the ip addresses for later reference 
 * this is a simple way to get all peers on our local subnet
 *
 * adapted from
 * http://ntrg.cs.tcd.ie/undergrad/4ba2/multicast/antony/example.html
 * listener.c -- joins a multicast group and echoes all data it receives from
 *                the group to its stdout...
 *
 * Antony Courtney,        25/11/94
 * Modified by: Frédéric Bastien (25/03/04)
 * to compile without warning and work correctly
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "napmulticast.h"

int main(int argc, char *argv[])
{
     struct sockaddr_in addr, saddr;
     int fd, nbytes;
     socklen_t addrlen;
     struct ip_mreq mreq;
     char msgbuf[NAPMSGLEN];
     u_int yes=1; 
     char *napgroup = NAP_GROUP;
     int napport = NAP_PORT;
     int c;

     while ((c = getopt(argc,argv,"g:p:d:h")) > 0) {
          switch(c) {
          case 'h': printf("%s -g{mcastgroup} -p{port} -d{peerdir} [-h]\n"
                           "\tListens for nap hosts on a local multicast group.\n"
                           "\t-g{mcastgroup} multicast group (%s)\n"
                           "\t-p{port} port to listen on (%d)\n"
                           "\t-h this help.\n",
                           argv[0],napgroup,napport
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
          default: printf("unknown option try -h for help\n"); return 1;
          }
     }

     /* create what looks like an ordinary UDP socket */
     if ((fd=socket(AF_INET,SOCK_DGRAM,0)) < 0) {
          perror("socket");
          return 1;
     }


    /* allow multiple sockets to use the same PORT number */
    if (setsockopt(fd,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(yes)) < 0) {
       perror("Reusing ADDR failed");
       return 1;
    }

     /* set up destination address */
     memset(&addr,0,sizeof(addr));
     addr.sin_family=AF_INET;
     addr.sin_addr.s_addr=htonl(INADDR_ANY);
     addr.sin_port=htons(napport);
     
     /* set up destination address */
     memset(&saddr,0,sizeof(saddr));
     saddr.sin_family=AF_INET;
     saddr.sin_addr.s_addr=0; /* needs to be filled in with other host's ip */
     saddr.sin_port=htons(napport+1);
     
     /* bind to receive address */
     if (bind(fd,(struct sockaddr *) &addr,sizeof(addr)) < 0) {
          perror("bind");
          return 1;
     }
     
     /* use setsockopt() to request that the kernel join a multicast group */
     mreq.imr_multiaddr.s_addr=inet_addr(napgroup);
     mreq.imr_interface.s_addr=htonl(INADDR_ANY);
     if (setsockopt(fd,IPPROTO_IP,IP_ADD_MEMBERSHIP,&mreq,sizeof(mreq)) < 0) {
          perror("setsockopt");
          return 1;
     }

     /* now just enter a read-print loop */
     printf("listening for other nap hosts: send messages to %s:%d\n", napgroup, napport);
     while (1) {
          addrlen=sizeof(addr);
          if ((nbytes=recvfrom(fd,msgbuf,NAPMSGLEN,0,
                               (struct sockaddr *) &addr, &addrlen)) < 0) {
               perror("recvfrom");
               return 1;
          }
          msgbuf[nbytes] = 0;
          printf("message from %s: %s\n",inet_ntoa(addr.sin_addr),msgbuf);

          saddr.sin_addr.s_addr=addr.sin_addr.s_addr; /* needs to be filled in with other host's ip */
          if (sendto(fd,NAP_ACK,strlen(NAP_ACK),0,(struct sockaddr *) &saddr,
                     sizeof(saddr)) < 0) {
               perror("sendto");
               return 1;
          }
     }
}

