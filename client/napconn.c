/**
 * cmpt433 final project nap time systems
 * Cobbled together by Cal Woodruff <cwoodruf@sfu.ca> and Tom Betz
 *
 * sockets
 * http://www.cs.utah.edu/dept/old/texinfo/glibc-manual-0.02/library_15.html
 *
 * unix domain socket code from 
 * http://www.thomasstover.com/uds.html
 * http://linux.die.net/man/2/bind
 *
 * select / accept documentation
 * http://linux.die.net/man/2/accept
 * http://www.softlab.ntua.gr/facilities/documentation/unix/unix-socket-faq/unix-socket-faq-4.html
 *
 * argument processing code from
 * http://www.gnu.org/s/hello/manual/libc/Example-of-Getopt.html 
 * 
 * posix threads
 * http://www.yolinux.com/TUTORIALS/LinuxTutorialPosixThreads.html
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
#include <pthread.h>
#include "nap.h"
#include "napfile.h"
/* #include "napclient.h" - now incorporated in nap.h */

/* global options */
int keepalive = NAPKEEPALIVE;
int port = NAPPORT;
int verbose = 0;
char *sockfile = NAPSOCK;
char *pwfile = NAPPWFILE;
char *host = NAPHOST;
/* global socket */
int bridge_fd = -1;

/* global locks */
pthread_mutex_t bridge_lock = PTHREAD_MUTEX_INITIALIZER;

/* function declarations */
int local_sock_connect(char *sockfile, struct sockaddr_un *address);
int bridge_sock_connect(char *host, int port, struct sockaddr_in *address);
int validate_bridge_sock (char *pwfile, int bridge_fd);
int listen_sock_connect(int port, struct sockaddr_in *address);

/* void bridge_client (char* host, int port); // reincorporated in main loop */

/* connection handlers */
void initiate_request(int localconn); 
void generate_response(int remoteconn);

/* thread function to revalidate bridge connection */
void *revalidate_bridge(void *i);

/* utility functions */
/* int connection_handler(int connection_fd); */
int send_str_sock(int fd, char *str); 
int max_array(int ary[], int count); 
int is_localsubnet(char *ip); 

/* http://www.steve.org.uk/Reference/Unix/faq_8.html */
void sig_chld(int signo)
{
  int status;
  waitpid(-1, &status, WNOHANG);
}

/* start */
int main(int argc, char **argv)
{
 /* for select */
 fd_set socklist;
 int retval;

 /* for fork */
 // pid_t child;

 /* sockets and connections */
 int listen_fd;
 int unix_fd;

 struct sockaddr_un unix_address;
 struct sockaddr_in listen_address;
 struct sockaddr_in bridge_address;
 socklen_t address_length = sizeof(struct sockaddr_un);
 pthread_t revalidate_bridge_thread;
 int unix_conn = -1, listen_conn = -1, bridge_conn = -1;

 /* for options */
 int c;

 /* for the child processes below - they need to be automatically cleared */
 signal(SIGCHLD,sig_chld);

 while ((c = getopt(argc, argv, "vhp:s:H:P:t:")) >= 0) {
  switch (c)
   {
   case 'h': printf(
    "%s -evh -p{pwfile} -s{socket file} -H{host} -P{port}\n"
    "\tProgram that maintains connection to the napbridge server"
    "and does the job of forwarding requests and responses.\n"
    "\tCreates a local unix socket that can be used to send requests to.\n"
    "\tListens for requests on its local subnet and will try connecting on"
    "local subnet if it see address is local.\n"
    "\t-v verbose output\n"
    "\t-h this help\n"
    "\t-t{keepalive} revalidate connection after keepalive seconds (%d)\n"
    "\t-p{pwfile} password file to use to validate with bridge (%s)\n"
    "\t-s{socket file} socket file to send commands to (%s)\n"
    "\t-H{host} napbridge host to connect to (%s)\n"
    "\t-P{port} napbridge port to connect to (%d)\n",
    argv[0], NAPKEEPALIVE, NAPPWFILE, NAPSOCK, host, port
    );
    return 0;

   case 'v': verbose = 1; break;
   case 'p': pwfile = optarg; break;
   case 's': sockfile = optarg; break;
   case 'H': host = optarg; break;
   case 'P': port = atoi(optarg); break;
   case 't': keepalive = atoi(optarg); break;
   default: 
    printf("unknown option %c\n", c); 
    abort();
   }
 }

/*
 // this is now implemented in main loop below
 child = fork();
 if(child == 0)
 {
  // initiate a connection to NAPHOST and wait for requests 
  bridge_client (host, port);
  return 0;
 }
*/

 /* TODO: allow the bridge socket to not exist and try reconnect in the background? */
 bridge_fd = bridge_sock_connect(host, port, &bridge_address);
 if (pthread_create(&revalidate_bridge_thread, NULL, revalidate_bridge, NULL)) {
  perror("could not create revalidate_bridge_thread");
  return 1;
 }
 if (pthread_detach(revalidate_bridge_thread)) {
  perror("error detaching revalidate_bridge_thread");
  return 1;
 }

 while (1) {
  printf("attempting to connect\n");
  bridge_fd = bridge_sock_connect(host, port,NULL);
  if (bridge_fd < 0) { 
   perror("can't connect to bridge socket"); 
   sleep(5); 
   continue; 
  }
  /* essential to validate with bridge or no communication possible - you can update the pwfile to fix */
  if ((retval = validate_bridge_sock(pwfile, bridge_fd))) { 
   fprintf(stderr, "error validating bridge: %d\n", retval); 
   sleep(5);
   continue;
  }
  printf("connected to bridge on %d\n", bridge_fd);

  unix_fd = local_sock_connect(sockfile, &unix_address); 
  if (unix_fd < 0) { perror("can't connect to local unix socket"); sleep(5); continue; }
  printf("connected to unix socket %s on %d\n", sockfile, unix_fd);
 
  listen_fd = listen_sock_connect(port, &listen_address);
  if (listen_fd < 0) { perror("can't connect to listen socket"); sleep(5); continue; }
  printf("connected to listen to %d locally on %d\n", port, listen_fd);

  /* must be reset every time select is run see:
     http://www.mapkreke.com/blog/2007/03/09/unix-using-select-with-multiple-sockets-in-c/ */
  FD_ZERO(&socklist); /* Always clear the structure first. */
  FD_SET(bridge_fd, &socklist);
  FD_SET(listen_fd, &socklist);
  FD_SET(unix_fd, &socklist);
  retval = select(FD_SETSIZE, &socklist, NULL, NULL, NULL);

  do {
   if (FD_ISSET(unix_fd, &socklist)) {
printf("got unix_fd req\n");
    if ((unix_conn = accept(unix_fd, 
                            (struct sockaddr *) &unix_address,
                            &address_length)) > -1)
    {
printf("initiate_request %d\n", unix_conn);
     initiate_request(unix_conn);
     close(unix_conn);
     unix_conn = -1;
    }
   }
   else if (FD_ISSET(listen_fd, &socklist)) {
printf("got listen_fd req\n");
    if ((listen_conn = accept(listen_fd, 
                              (struct sockaddr *) &listen_address,
                              &address_length)) > -1)
    {
printf("generate_response %d\n", listen_conn);
     generate_response(listen_conn);
     close(listen_conn);
     listen_conn = -1;
    }
   }
   else if (FD_ISSET(bridge_fd, &socklist)) {
printf("got bridge_fd req\n");
    pthread_mutex_lock(&bridge_lock);
    if ((bridge_conn = accept(bridge_fd, 
                              (struct sockaddr *) &bridge_address,
                              &address_length)) > -1)
    {
printf("generate_response %d\n", bridge_conn);
     generate_response(bridge_conn);
     close(bridge_conn);
     bridge_conn = -1;
    }
    pthread_mutex_unlock(&bridge_lock);
   }

   FD_ZERO(&socklist); /* Always clear the structure first. */
   FD_SET(bridge_fd, &socklist);
   FD_SET(listen_fd, &socklist);
   FD_SET(unix_fd, &socklist);
  } while ((retval = select(FD_SETSIZE, &socklist, NULL, NULL, NULL)) >= 0);
  if (retval < 0) {
   perror("select error");
  }
 }
 close(unix_fd);
 close(listen_fd);
 close(bridge_fd);
 unlink(sockfile);
 return 0;
}
/*
 // example function for handling input from a connection
int connection_handler(int connection_fd)
{ 
 int nbytes;
 char buffer[NAPBUFFSIZE];

 nbytes = read(connection_fd, buffer, NAPBUFFSIZE);
 buffer[nbytes] = 0;

 printf("MESSAGE FROM CLIENT: %s\n", buffer);
 nbytes = snprintf(buffer, NAPBUFFSIZE, "hello from the server");
 write(connection_fd, buffer, nbytes);
 
 close(connection_fd);
 return 0;
}
*/

int is_localsubnet(char *ip) 
{
 return 0;
}

/**
 * get initial command from local unix socket
 * this will be a single line string
 * @param localconn
 */
void initiate_request(int localconn) 
{
 int nbytes, islocal = 0, fd;
 char *cmd, *ip;
 char buffer[NAPBUFFSIZE],copy[NAPBUFFSIZE],response[NAPBUFFSIZE];
 struct sockaddr_in address;

 nbytes = read(localconn, buffer, NAPBUFFSIZE);
 buffer[nbytes] = 0;
 strcpy(copy,buffer);

 if (nbytes > 0) {
  cmd = strtok(copy," ");
  ip = strtok(NULL," ");
  if (ip != NULL) islocal = is_localsubnet(ip);

  if (islocal) {
   printf("sending %s to local subnet - will connect to %s\n", buffer, ip);
   fd = bridge_sock_connect(ip, port, &address);
   if (fd < 0) {
    perror("can't connect");
    /* if we can't connect to node locally connecting via bridge may work */
    fd = bridge_fd;
   }
  } else {
   printf("sending %s to bridge server for ip %s\n", buffer, ip);
   fd = bridge_fd;
  }

  if (send_str_sock(fd,buffer) == 0) {
   /* just forward along what we get back for now */
   while ((nbytes = recv(fd, response, NAPBUFFSIZE, 0)) > 0) {
    if (nbytes == -1) {
     perror("validate_bridge_sock: error receiving");
     return;
    }
    write(localconn,response,nbytes);
   }
  }
  if (fd != bridge_fd) close(fd);
 }
}

void generate_response(int remoteconn)
{
 int nbytes, bytessent;
 char *resp, *bytestr, *respcode, *fname;
 char **lines, **respline;
 char response[NAPBUFFSIZE],copy[NAPBUFFSIZE];

 nbytes = read(remoteconn, response, NAPBUFFSIZE);
 if (nbytes == -1) {
  perror("validate_bridge_sock: can't receive");
  return;
 }
 if (nbytes == 0) return; /* nothing to read ? */
 /* this reads each line and parses the first line */
 strcpy(copy,response);
 resp = strtok_r(copy,"\r\n",lines);
 if (respline) {
  respcode = strtok_r(resp," ",respline);
  if (strcmp(respcode,NAPRESPONSE) == 0) {
   bytestr = strtok_r(NULL,"\r\n");
   bytessent = atoi(bytestr);
   fname = strtok_r
   nbytes = read(remoteconn, response, NAPBUFFSIZE);
 }
}

/**
 * thread functiont that handles revalidating with bridge server
 * does this on a regular schedule bridge_lock should be used above
 * to protect writing to the bridge_fd
 * @param the socket handle bridge_fd
 * uses host, port and keepalive global variables
 */
void *revalidate_bridge(void *i) 
{
 int retval;
 struct sockaddr_in address;

 printf("starting revalidate_bridge: keepalive %d\n", keepalive);
 sleep(keepalive);

 while (1) {

  pthread_mutex_lock(&bridge_lock);
  retval = validate_bridge_sock(pwfile, bridge_fd);
  if (retval > 0) {
   bridge_fd = bridge_sock_connect(host, port, &address);
  }
  pthread_mutex_unlock(&bridge_lock);

  sleep(keepalive);
 }
}

/**
 * finds maximum positive value in an array
 * @param array of ints
 * @param count
 * @return largest postive value
 */
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
int validate_bridge_sock (char *pwfile, int bridge_fd) {
 if (bridge_fd == -1) {
  return -1;
 }
 char password[30] = {0};
 int result = nap_readFile (pwfile, password, 30);
 if (result == -1) {
  printf ("Can't find validation password\n");
  return -1;
 }

 char validateString[50] = {0};
 sprintf (validateString, "%s %s\n", NAPCOMMAND_VALIDATE, password);
 if ((result = send_str_sock(bridge_fd, validateString))) return result; 

 char message[50] = {0};
 result = recv (bridge_fd, message, 50, 0);
 if (result == -1) {
  perror("validate_bridge_sock: can't receive");
  if (errno == EDESTADDRREQ) {
   return 1;
  }
  return -1;
 }
 nap_chomp(message);
 if (strcmp (message, NAPRESPONSE_VALID) == 0) {
  return 0;
 }
 fprintf(
  stderr,
  "validate_bridge_sock: invalid password (response %s vs expected %s)\n",
  message,
  NAPRESPONSE_VALID
 );
 return -1;
}

/**
 * send string to socket and see if send worked
 * @param file descriptor
 * @param string to send
 * returns -1 connect failure, 1 can't send, 0 ok
 */
int send_str_sock(int fd, char *str) 
{
 int result;

 result = send(fd, str, strlen(str), 0);
 if (result == -1) {
  perror("can't send");
  if (errno == EDESTADDRREQ) {
   return 1;
  }
  return -1;
 }
 return 0;
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
 static int newsocket = -1; 
 struct sockaddr_in newaddr;

 // allow function to be called multiple times on a live socket
 // use serverAddr to indicate if we want to reuse it
 if (serverAddr == NULL) {
  if (newsocket >= 0) return newsocket;
  serverAddr = &newaddr;
 } else {
  if (newsocket >= 0) close(newsocket);
 }

 newsocket = socket(PF_INET, SOCK_STREAM, 0);
 
 //initialize the bytes of serverAddr to all zero
 memset (serverAddr, 0, sizeof(struct sockaddr_in));
 
 //specify IPv4 address type
 serverAddr->sin_family = PF_INET;
 
 //specify server address using MACRO (adjust to real value)
 inet_pton (PF_INET,host,&(serverAddr->sin_addr));
 
 //server port number
 serverAddr->sin_port = htons (port);

 //connect
 int result = connect (newsocket, (struct sockaddr*)serverAddr, sizeof (struct sockaddr));

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

