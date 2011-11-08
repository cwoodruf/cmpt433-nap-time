/**
 * cmpt433 final project nap-time-systems group
 * Cobbled together by Cal Woodruff <cwoodruf@sfu.ca>
 * taken almost verbatim from
 * http://www.thomasstover.com/uds.html
 * and from
 * http://www.gnu.org/s/hello/manual/libc/Example-of-Getopt.html
 */
#include <ctype.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "nap.h"

int main(int argc, char **argv)
{
 struct sockaddr_un address;
 int  socket_fd, nbytes;
 char buffer[BUFFSIZE];
 char *sockfile = "./napclient.sock";
 int c;
 int verbose = 0;
 int emulate = 0;
 
 while ((c = getopt(argc, argv, "vhs:"))) {
  switch (c)
   {
   case 'h': printf(
    "%s -vh -s{socket file}\n"
    "\tSimple client that can forward commands to the napclient program\n"
    "\t-v verbose output\n"
    "\t-h this help\n"
    "\t-s{socket file} socket file to send commands to (%s)\n",
    argv[0], sockfile
    );
    return 0;

   case 'e': emulate = 1; break;
   case 'v': verbose = 1; break;
   case 's': sockfile = optarg; break;
   default: abort();
   }
 }

 socket_fd = socket(PF_UNIX, SOCK_STREAM, 0);
 if(socket_fd < 0)
 {
  printf("socket() failed\n");
  return 1;
 }

 /* start with a clean address structure */
 memset(&address, 0, sizeof(struct sockaddr_un));
 
 address.sun_family = AF_UNIX;
 snprintf(address.sun_path, UNIX_PATH_MAX, "%s", sockfile);

 if(connect(socket_fd, 
            (struct sockaddr *) &address, 
            sizeof(struct sockaddr_un)) != 0)
 {
  printf("connect() failed\n");
  return 1;
 }

 nbytes = snprintf(buffer, 256, "hello from a client");
 write(socket_fd, buffer, nbytes);
 
 nbytes = read(socket_fd, buffer, 256);
 buffer[nbytes] = 0;

 printf("MESSAGE FROM SERVER: %s\n", buffer);

 close(socket_fd);

 return 0;
}
