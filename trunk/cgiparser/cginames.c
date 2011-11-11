/**
 * cmpt433 nap time systems
 * simple cgi parser that returns a list of names in a cgi string
 * @author Cal Woodruff <cwoodruf@sfu.ca>
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) 
{
	char *tok;
	char *cgi;

	if (argc != 2) {
		fprintf(stderr,"Need a cgi string to parse!\n");
		return 1;
	}
	if ((cgi = malloc(strlen(argv[1]))) == NULL) {
		perror("cgi malloc");
		return 1;
	}
	strcpy(cgi,argv[1]);

	tok = strtok(cgi,"&=");

	while (tok != NULL) {
		printf("%s\n", tok);
		tok = strtok(NULL,"&");
		tok = strtok(NULL,"=");
	}
}	
