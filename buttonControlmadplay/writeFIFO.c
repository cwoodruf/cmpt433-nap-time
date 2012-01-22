#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

int pipe1;

int main(int argc, char**argv)
{
	if (argc < 3) {
		fprintf(stderr,"need 2 arguments fifo and char to send to it");
		return 1;
	}
	if ((pipe1 = open(argv[1], O_WRONLY)) == -1) {
		perror("error opening pipe");
		return 1;
	}
	write(pipe1, argv[2], 1);
	close(pipe1);
	return 0;
}

