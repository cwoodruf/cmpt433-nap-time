/**
 * cmpt433 final project nap time systems
 * Cobbled together by Cal Woodruff <cwoodruf@sfu.ca>
 *
 * unix domain socket code from 
 * http://www.thomasstover.com/uds.html
 * http://linux.die.net/man/2/bind
 *
 * select / accept documentation
 * http://linux.die.net/man/2/accept
 * http://www.softlab.ntua.gr/facilities/documentation/unix/unix-socket-faq/unix-socket-faq-4.html
 *
 * argument process code from
 * http://www.gnu.org/s/hello/manual/libc/Example-of-Getopt.html 
 *
 * inet code from Tom Betz
 */
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include "nap.h"
#include "napfile.h"
/* #include "napclient.h" - now incorporated in nap.h */

int local_sock_connect(char *sockfile, struct sockaddr_un *address);
int bridge_sock_connect(char *host, int port, struct sockaddr_in *address);
int listen_sock_connect(int port, struct sockaddr_in *address);

void bridge_client (char* host, int port);
int validate_bridge_sock (int bridge_fd);

int connection_handler(int connection_fd);
int max_array(int ary[], int count); 

/* http://www.steve.org.uk/Reference/Unix/faq_8.html */
void sig_chld(int signo)
{
  int status;
  waitpid(-1, &status, WNOHANG);
}

int main(int argc, char **argv)
{
 /* for select */
 fd_set socklist;
 int retval, nfds;

 /* for fork */
 pid_t child;

 /* sockets and connections */
 int listen_fd;
 int unix_fd;
 int bridge_fd;

 struct sockaddr_un unix_address;
 struct sockaddr_in listen_address;
 struct sockaddr_in bridge_address;
 socklen_t address_length = sizeof(struct sockaddr_un);

#define FDCOUNT 2
 int fds[FDCOUNT];
 int connection_fd;

 /* options */
 char *sockfile = NAPSOCK;
 char *host = NAPHOST;
 int port = NAPPORT;
 int c;
 int verbose = 0;
 int emulate = 0;

 /* for the child processes below - they need to be automatically cleared */
 signal(SIGCHLD,sig_chld);

 while ((c = getopt(argc, argv, "vhs:H:p:"))) {
  switch (c)
   {
   case 'h': printf(
    "%s -evh -s{socket file} -h{host} -p{port}\n"
    "\tSimple client that can forward commands to the napclient program\n"
    "\t-e emulate the connection with the host\n"
    "\t-v verbose output\n"
    "\t-h this help\n"
    "\t-s{socket file} socket file to send commands to (%s)\n"
    "\t-h{host} napbridge host to connect to (%s)\n"
    "\t-h{port} napbridge port to connect to (%d)\n",
    argv[0], sockfile, host, port
    );
    return 0;

   case 'e': emulate = 1; break;
   case 'v': verbose = 1; break;
   case 's': sockfile = optarg; break;
   case 'H': host = optarg; break;
   case 'p': port = atoi(optarg); break;
   default: abort();
   }
 }

/*
 child = fork();
 if(child == 0)
 {
  // initiate a connection to NAPHOST and wait for requests 
  bridge_client (host, port);
  return 0;
 }
*/

 while (1) {
  /* TODO: allow the bridge socket to not exist and try reconnect in the background */
  bridge_fd = bridge_sock_connect(host, port, &bridge_address);
  if (bridge_fd < 0) { 
   perror("can't connect to bridge socket"); 
   sleep(5); 
   continue; 
  }
  /* essential to validate with bridge or no communication possible */
  if ((retval = validate_bridge_sock(bridge_fd))) { 
   fprintf(stderr, "error validating bridge: %d\n", retval); 
   return 1;
  }

  unix_fd = local_sock_connect(sockfile, &unix_address); 
  if (unix_fd < 0) { perror("can't connect to local unix socket"); sleep(5); continue; }
 
  listen_fd = listen_sock_connect(port, &listen_address);
  if (listen_fd < 0) { perror("can't connect to listen socket"); sleep(5); continue; }

  FD_ZERO(&socklist); /* Always clear the structure first. */
  FD_SET(listen_fd, &socklist);
  FD_SET(unix_fd, &socklist);

  fds[0] = unix_fd;
  fds[1] = listen_fd;
  nfds = 1 + max_array(fds,FDCOUNT);
 
  /* this will alert us when there is something to read on a socket */
  while ((retval = select(nfds, &socklist, NULL, NULL, NULL)) >= 0) {
   if (FD_ISSET(unix_fd, &socklist)) {
    if ((connection_fd = accept(unix_fd, 
                                (struct sockaddr *) &unix_address,
                                &address_length)) > -1)
    {
     child = fork();
     if(child == 0)
     {
      /* now inside newly created connection handling process */
      /* close listening socket in child process */
      return connection_handler(connection_fd);
     }
     /* still inside server process */
     close(connection_fd);
    }
   }
   else if (FD_ISSET(listen_fd, &socklist)) {
    if ((connection_fd = accept(listen_fd, 
                                (struct sockaddr *) &listen_address,
                                &address_length)) > -1)
    {
     child = fork();
     if(child == 0)
     {
      /* now inside newly created connection handling process */
      /* close listening socket in child process */
      return connection_handler(connection_fd);
     }
     /* still inside server process */
     close(connection_fd);
    }
   }
   else if (FD_ISSET(bridge_fd, &socklist)) {
    if ((connection_fd = accept(bridge_fd, 
                                (struct sockaddr *) &bridge_address,
                                &address_length)) > -1)
    {
     child = fork();
     if(child == 0)
     {
      /* now inside newly created connection handling process */
      /* close bridgeing socket in child process */
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
 close(listen_fd);
 unlink(sockfile);
 return 0;
}

/* - causing compilation errors: I've incorporated this into the main while loop
void bridge_client (char* host, int port) {
 int bridge_fd = bridge_sock_connect(host, port, &bridge_address);
 while (bridge_fd < 0) {
  perror("can't connect to bridge socket");
  sleep(5);
  bridge_fd = bridge_sock_connect (host, port);
 }

 fd_set rfds;
 struct timeval tv;
 int result;

 FD_ZERO (&rfds);
 FD_SET (bridge_fd, &rfds);

 // wait up to four minutes and 30 seconds
 tv.tv_sec = 270;
 tv.tv_usec = 0;
 char message[50];
 while (1) {
  result = select (1, &rfds, NULL, NULL, &tv);
  if (result == -1) {
   printf ("bridge client: select error\n");
  } else if (result) {
   //receive and send some data to server
  } else {
   if (validate_bridge_sock (bridge_fd) == 1) {
    int result = bridge_sock_connect (host, port);
    while (result == -1) {
     sleep (5);
     result = bridge_sock_connect (host, port);
    }
    bridge_fd = result;
   }
  }
 }
}
*/

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
int local_sock_connect(char *sockfile, struct sockaddr_un *address) 
{
 int unix_fd;

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
 snprintf(address->sun_path, UNIX_PATH_MAX, "%s", sockfile);

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

/*
 * CMPT433 naplclient code
 * T.E. Betz
 * 6 November 2011
 */
/**
 * returns 1 if validation fails because of no connection
 * returns 0 if success
 * return -1 otherwise
 */
int validate_bridge_sock (int bridge_fd) {
 int remoteSocket = bridge_fd;
 if (remoteSocket == -1) {
  return -1;
 }
 char password[30] = {0};
 int result = nap_readFile (NAPPASSWORD_FILE_LOCATION, password, 30);
 if (result == -1) {
  printf ("Can't find validation password\n");
  return -1;
 }

 char validateString[50] = {0};
 sprintf (validateString, "%s %s\n", NAPCOMMAND_VALIDATE, password);
 result = send(remoteSocket, validateString, strlen(validateString), 0);
 if (result == -1) {
  if (errno == EDESTADDRREQ) {
   return 1;
  }
  return -1;
 }

 char message[50] = {0};
 result = recv (remoteSocket, message, 50, 0);
 if (result == -1) {
  if (errno == EDESTADDRREQ) {
   return 1;
  }
  return -1;
 }
 
 if (strcmp (message, NAPRESPONSE_VALID) == 0) {
  return 0;
 } else {
  return -1;
 }
}

/**
 * connect with the external napbridge server as a client
 * we'll get commands from them and should try and keep the connection open
 *
 * @param host - host running napbridge
 * @param port - port to try and connect to
 * @return file descriptor for socket
 */
int bridge_sock_connect(char *host, int port, struct sockaddr_in *serverAddr)
{
 //create a socket for IPv4 protocol and TCP
 int newsocket = socket(PF_INET, SOCK_STREAM, 0);
 
 //initialize the bytes of serverAddr to all zero
 memset (serverAddr, 0, sizeof(struct sockaddr_in));
 
 //specify IPv4 address type
 serverAddr->sin_family = PF_INET;
 
 //specify server address using MACRO (adjust to real value)
 inet_pton (PF_INET,host,&(serverAddr->sin_addr));
 
 //server port number
 serverAddr->sin_port = htons (port);

 //connect
 int result = connect (newsocket, (struct sockaddr*)serverAddr, sizeof (serverAddr));

 if (result == -1) {
  perror ("Client: bridge connection failed.\n");
  return -1;
 }
 return newsocket;
}

/**
 * create connection for requests on the local subnet
 * @param port - port to listen on
 * @return file descriptor for incoming connections 
 */
int listen_sock_connect(int port,struct sockaddr_in *address) 
{
  // get a tcp/ip socket
  int mysocket = socket (PF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (mysocket == -1) {
    printf ("Cannot open socket\n");
    return -1;
  }

  memset (address, 0, sizeof (struct sockaddr_in));
  address->sin_family = PF_INET;
  address->sin_addr.s_addr = htonl (INADDR_ANY);
  address->sin_port = htons (port);
  bind (mysocket, (struct sockaddr *)address, sizeof (struct sockaddr_in));

  int result = listen (mysocket, 1);
  if (result == -1) {
    printf ("Failed to listen\n");
    return -1;
  }

  return mysocket;
}

