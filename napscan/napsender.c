/* 
 * 
 * adapted from
 * sender.c -- multicasts "hello, world!" to a multicast group once a second
 * http://ntrg.cs.tcd.ie/undergrad/4ba2/multicast/antony/example.html
 * Antony Courtney,	25/11/94
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include "napmulticast.h"

int main(int argc, char *argv[])
{
     struct sockaddr_in addr;
     int fd/*, cnt */;
     /* struct ip_mreq mreq; */
     char *message="Hello, World!";

     /* create what looks like an ordinary UDP socket */
     if ((fd=socket(AF_INET,SOCK_DGRAM,0)) < 0) {
	  perror("socket");
	  return 1;
     }

     /* set up destination address */
     memset(&addr,0,sizeof(addr));
     addr.sin_family=AF_INET;
     addr.sin_addr.s_addr=inet_addr(NAP_GROUP);
     addr.sin_port=htons(NAP_PORT);
     
     /* now just sendto() our destination! */
     while (1) {
	  if (sendto(fd,message,strlen(message),0,(struct sockaddr *) &addr,
		     sizeof(addr)) < 0) {
	       perror("sendto");
	       return 1;
	  }
	  sleep(1);
     }
     return 0;
}

