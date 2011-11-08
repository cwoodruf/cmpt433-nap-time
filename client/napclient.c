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
#include <pthread.h>
#include <errno.h>
#include "napclient.h"
#include "file.h"

#define NODE_LIFE 600
#define SERVER_ADDRESS "127.0.0.1" //change the value of this to actual server IP
#define SERVER_PORT 29533
#define RESPONSE_VALID "VALID"
#define COMMAND_VALIDATE "validate"

int remoteSocket = -1;
int localSocket = -1;

int getPassword (char* password, int count) {
	int result = readFile ("password", password, count);
	return result;
}

/**
* returns 1 for no connection
* return 0 upon success
* return -1 otherwise
**/
int validate () {
	if (remoteSocket == -1) {
		return -1;
	}
	char password[30] = {0};
	int result = getPassword (password, 30);
	if (result == -1) {
		printf ("Can't find validation password\n");
		return -1;
	}

	char validateString[50] = {0};
	sprintf (validateString, "%s %s\n", COMMAND_VALIDATE, password);
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
	
	if (strcmp (message, RESPONSE_VALID) == 0) {
		return 0;
	} else {
		return -1;
	}
}

void remoteConnect () {
	struct sockaddr_in serverAddr;

	if (remoteSocket == -1) {
		return;
	}

	//create a socket for IPv4 protocol and TCP
	remoteSocket = socket (PF_INET, SOCK_STREAM, 0);

	//initialize the bytes of serverAddr to all zero
	bzero (&serverAddr, sizeof (serverAddr));

	//specify IPv4 address type
	serverAddr.sin_family = PF_INET;

	//specify server address using MACRO (adjust to real value)
	inet_pton (PF_INET, SERVER_ADDRESS, &(serverAddr.sin_addr));

	//server port number
	serverAddr.sin_port = htons (SERVER_PORT);

	//connect
	int result = connect (remoteSocket, (struct sockaddr*)&serverAddr, sizeof (serverAddr));

	while (result == -1) {
		printf ("Can't connect to %s:%s. Retry in 5 seconds\n", SERVER_ADDRESS, SERVER_PORT);
		sleep (5);
		result = connect (remoteSocket, (struct sockaddr*)&serverAddr, sizeof (serverAddr));
	}

	printf ("Established connection to %s:%s\n", SERVER_ADDRESS, SERVER_PORT);
}

void* periodicValidation (void* ptr) {
	sleep (NODE_LIFE / 2);
	int result;

	while (remoteSocket != -1) {
		result = validate ();
		if (result == 1) { //no connection
			remoteConnect ();
			validate ();
		}
		sleep (NODE_LIFE / 2);
	}
}

void main () {
	remoteConnect ();
	
	pthread_t validationThread;
	validationThread = pthread_create (&validationThread, NULL, periodicValidation, (void*)NULL);

	if (fileExists (

	shutdown (newsocket, 2);
	close (newsocket);
	printf ("Client: Exit\n");
}
