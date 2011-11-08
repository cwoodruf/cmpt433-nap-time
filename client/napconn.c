/**
 * cmpt433 final project nap time systems
 * Cobbled together by Cal Woodruff <cwoodruf@sfu.ca>
 * unix domain socket code from 
 * http://www.thomasstover.com/uds.html
 * select / accept documentation
 * http://linux.die.net/man/2/accept
 * http://www.softlab.ntua.gr/facilities/documentation/unix/unix-socket-faq/unix-socket-faq-4.html
 * argument process code from
 * http://www.gnu.org/s/hello/manual/libc/Example-of-Getopt.html 
 * inet code from Tom Betz
 */
#include <ctype.h>
#include <stdio.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include "nap.h"

int local_sock_connect(char *sockfile, struct sockadd_un *address);
int bridge_sock_connect(char *host, int port, struct sockadd_un *address);
int listen_sock_connect(int port, struct sockadd_un *address);

int connection_handler(int connection_fd);
int max_array(int ary[], int count); 

int main(int argc, char **argv)
{
 /* for select */
 fd_set socklist;
 int retval, nfds;

 /* sockets and connections */
 int bridge_fd;
 int listen_fd;
 int unix_fd;

 struct sockaddr_un unix_address;
 struct sockaddr_un bridge_address;
 struct sockaddr_un listen_address;
 socklen_t address_length = sizeof(struct sockaddr_un);

#define FDCOUNT 3
 int fds[FDCOUNT-1];
 int connection_fd;

 /* options */
 char *sockfile = NAPSOCK;
 char *host = NAPHOST;
 int port = NAPPORT;
 int c, help = 0;
 int verbose = 0;
 int emulate = 0;

 while ((c = getopt(argc, argv, "vhs:h:p:"))) {
  switch (c)
   {
   case 'h': printf(
    "%s -evh -s{socket file} -h{host} -p{port}\n"
    "\tSimple client that can forward commands to the napclient program\n"
    "\t-e emulate the connection with the host\n";
    "\t-v verbose output\n"
    "\t-h this help\n"
    "\t-s{socket file} socket file to send commands to (%s)\n",
    "\t-h{host} napbridge host to connect to (%s)\n",
    "\t-h{port} napbridge port to connect to (%d)\n",
    argv[0], sockfile, host, port
    );
    return 0;

   case 'e': emulate = 1; break;
   case 'v': verbose = 1; break;
   case 's': sockfile = optarg; break;
   case 'h': host = optarg; break;
   case 'p': port = atoi(optarg); break;
   default: abort();
   }
 }
 

 /**
  * this is different in that we have to listen on 3 interfaces:
  * 1) listen locally for connections on the NAPPORT port
  * 2) initiate a connection to NAPHOST and wait for requests
  * 3) listen on our local unix domain socket for commands
  */
 while (1) {
  unix_fd = local_sock_connect(char *sockfile, &unix_address); 
  if (unix_fd < 0) { perror("can't connect to local unix socket"); sleep 5; continue; }
 
  bridge_fd = bridge_sock_connect(char *host, int port, &bridge_address);
  if (bridge_fd < 0) { perror("can't connect to bridge socket"); sleep 5; continue; }
 
  listen_fd = listen_sock_connect(int port, &listen_address);
  if (listen_fd < 0) { perror("can't connect to listen socket"); sleep 5; continue; }

  FD_ZERO(&socklist); /* Always clear the structure first. */
  FD_SET(napbridge_fd, &socklist);
  FD_SET(inet_fd, &socklist);
  FD_SET(unix_fd, &socklist);

  fds[0] = unix_fd;
  fds[1] = bridge_fd;
  fds[2] = listen_fd;
  nfds = 1 + maxarray(fds,FDCOUNT);
 
  /* this will alert us when there is something to read on a socket */
  while ((retval = select(nfds, &socklist, NULL, NULL, NULL)) >= 0) {
   if (FD_ISSET(unix_fd, &rfds)) {
    if ((connection_fd = accept(unix_fd, 
                                (struct sockaddr *) &unix_address,
                                &address_length)) > -1)
    {
     child = fork();
     if(child == 0)
     {
      /* now inside newly created connection handling process */
      return connection_handler(connection_fd);
     }
     /* still inside server process */
     close(connection_fd);
    }
   }
   else if (FD_ISSET(listen_fd, &rfds)) {
    if ((connection_fd = accept(listen_fd, 
                                (struct sockaddr *) &listen_address,
                                &address_length)) > -1)
    {
     child = fork();
     if(child == 0)
     {
      /* now inside newly created connection handling process */
      return connection_handler(connection_fd);
     }
     /* still inside server process */
     close(connection_fd);
    }
   }
   else if (FD_ISSET(bridge_fd, &rfds)) {
    if ((connection_fd = accept(bridge_fd, 
                                (struct sockaddr *) &bridge_address,
                                &address_length)) > -1)
    {
     child = fork();
     if(child == 0)
     {
      /* now inside newly created connection handling process */
      return connection_handler(connection_fd);
     }
     /* still inside server process */
     close(connection_fd);
    }
   }
  }
  if (retval < 0) {
   perror("select error");
  }
 }
 close(unix_fd);
 close(bridge_fd);
 close(listen_fd);
 unlink(sockfile);
 return 0;
}

/**
 * example function for handling input from a connection
 */
int connection_handler(int connection_fd)
{
 int nbytes;
 char buffer[BUFFSIZE];

 nbytes = read(connection_fd, buffer, BUFFSIZE);
 buffer[nbytes] = 0;

 printf("MESSAGE FROM CLIENT: %s\n", buffer);
 nbytes = snprintf(buffer, BUFFSIZE, "hello from the server");
 write(connection_fd, buffer, nbytes);
 
 close(connection_fd);
 return 0;
}

int max_array(int ary[], int count) 
{
 int i, max = 0;
 for (i=0; i<count; i++) {
  if (ary[i] < 0) continue;
  if (ary[i] > max) max = ary[i];
 }
 return max;
}
/**
 * connects to our local unix command socket
 * @param sockfile - name of socket file to make
 * @return file descriptor for unix domain socket
 */
int local_sock_connect(char *sockfile) 
{
 int unix_fd;
 pid_t child;

 unix_fd = socket(PF_UNIX, SOCK_STREAM, 0);
 if(unix_fd < 0)
 {
  printf("socket() failed\n");
  return -1;
 } 

 unlink(sockfile);

 /* start with a clean address structure */
 memset(address, 0, sizeof(struct sockaddr_un));

 address->sun_family = AF_UNIX;
 snprintf(address->sun_path, UNIX_PATH_MAX, sockfile);

 if(bind(unix_fd, 
         (struct sockaddr *) address, 
         sizeof(struct sockaddr_un)) != 0)
 {
  printf("bind() failed\n");
  return -1;
 }

 if(listen(unix_fd, 5) != 0)
 {
  printf("listen() failed\n");
  return -1;
 }
 return unix_fd;
}

/**
 * connect with the external napbridge server
 * @param host - host running napbridge
 * @param port - port to try and connect to
 * @return file descriptor for socket
 */
int bridge_connect(char *host, int port)
{
 return -1;
}

/**
 * create connection for requests on the local subnet
 * @param port - port to listen on
 * @return file descriptor for incoming connections 
 */
int listen_sock_connect(int port) 
{
 return -1;
}

