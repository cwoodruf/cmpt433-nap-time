#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include <fstream>
void getMusicList (std::string& list);

int main () {
  sockaddr_in serverAddr;

  // get a tcp/ip socket
  int mysocket = socket (PF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (mysocket == -1) {
    perror ("Cannot open socket\n");
    exit (EXIT_FAILURE);
  }

  bzero (&serverAddr, sizeof (serverAddr));
  serverAddr.sin_family = PF_INET;
  serverAddr.sin_addr.s_addr = htonl (INADDR_ANY);
  serverAddr.sin_port = htons (10001);
  bind (mysocket, (sockaddr *)&serverAddr, sizeof (serverAddr));

  int result = listen (mysocket, 1);
  if (result == -1) {
    perror ("Failed to listen\n");
    exit (EXIT_FAILURE);
  }

  while (1) {
    int client = accept (mysocket, (sockaddr*)NULL, NULL);

    printf ("Server: New connection. Sending data\n");
    std::string message;
    getMusicList (message);

    result = send (client, message.c_str (), message.length (), 0);
    if (result == -1) {
      perror ("Failed to send data\n");
      exit (EXIT_FAILURE);
    }
    sleep (1);

    shutdown (client, 2);
    close (client);
    printf ("Server: Close connection\n");
  }

  close(mysocket);
  return 0;
}

void getMusicList (std::string& list) {
  list.clear ();
  system ("./getmusiclist.sh > temp");
  
  std::ifstream input ("temp");
  if (input.is_open () == false) {
    list = "no file\n";
    return;
  }

  std::string line;
  while (input.eof () == false) {
    getline (input, line);
    if (line.find (".mp3") != std::string::npos) {
      list += line.substr (1) + "\n"; //remove the '.' in front
    }
  }
}
