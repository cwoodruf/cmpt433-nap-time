#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <QString>
#include <QProcess>
#include <QStringList>

void getMusicList (const char* server, QString& musicList) {

  sockaddr_in serverAddr;

  int newsocket = socket (PF_INET, SOCK_STREAM, 0);
  bzero (&serverAddr, sizeof (serverAddr));
  serverAddr.sin_family = PF_INET;
  inet_pton (PF_INET, server, &(serverAddr.sin_addr));
  serverAddr.sin_port = htons (10001);

  int result = connect (newsocket, (sockaddr*)&serverAddr, sizeof (serverAddr));
  printf ("Client: Connection established. Starting to receive data..\n");
  char message[1000];

  result = recv (newsocket, message, 1000, 0);
  printf ("Client: Message received: %s\n", message);
  musicList = message;

  shutdown (newsocket, 2);
  close (newsocket);
  printf ("Client: Exit\n");
}

