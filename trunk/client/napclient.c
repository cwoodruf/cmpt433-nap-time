/*
*	CMPT433 naplclient code
*	T.E. Betz
*	6 November 2011
*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "napclient.h"

void main () {
	struct sockaddr_in serverAddr;
	//incoming message storage
	char message[250];
	  
	//create a socket for IPv4 protocol and TCP
	int newsocket = socket (PF_INET, SOCK_STREAM, 0);

	//initialize the bytes of serverAddr to all zero
	bzero (&serverAddr, sizeof (serverAddr));

	//specify IPv4 address type
	serverAddr.sin_family = PF_INET;

	//specify server address using MACRO (adjust to real value)
	inet_pton (PF_INET, SERVER_ADDRESS, &(serverAddr.sin_addr));

	//server port number
	serverAddr.sin_port = htons (SERVER_PORT);

	//connect
	int result = connect (newsocket, (struct sockaddr*)&serverAddr, sizeof (serverAddr));
	//if connection fails, reconnect every five seconds
	while (result == -1) {
		printf ("Client: Connection failed. Retry in 5 seconds\n");
		sleep (5);
		result = connect (newsocket, (struct sockaddr*)&serverAddr, sizeof (serverAddr));
	}

	printf ("Client: Connection established.\n");

	//send a message asking for list of peers
	send(newsocket, (const void *)COMMAND_PEERS, strlen(COMMAND_PEERS), 0);

	recv (newsocket, message, 250, 0);
	printf ("Client: Message received: %s\n", message);

	shutdown (newsocket, 2);
	close (newsocket);
	printf ("Client: Exit\n");
}
