/**
 * cmpt433 nap time systems
 * simple cgi parsing program
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void cgiprintdecode(char *value, size_t len);

int quiet = 0;

int main(int argc, char **argv) 
{
	int c;
	char *name = "";
	char *value, *end;
	char *cgi;
	while ((c = getopt(argc,argv,"qhn:")) != -1) {
		switch(c) {
		case 'h':
			printf(
				"Usage: %s [-q] -n{var name} {cgi string}\n"
				"\tPrint parsed variable output from a cgi string to stdout.\n"
				"\tReturns 0 if name is found, 2 if not and 1 on error.\n"
				"\t-h this help output\n"
				"\t-q suppress printing error output\n"
				"\t-n{var name} variable name (including =) to extract from cgi string.\n",
				argv[0]
			);
		return 0;
		case 'n':  
			if (strcmp(optarg,":")) name = optarg;
			
		break;
		case 'q': quiet = 1; break;
		default: 
			fprintf(stderr,"Need a -n option try -h for usage\n");
		return 1;
		}
	}
	if (optind > argc) {
		if (!quiet) fprintf(stderr,"Need a cgi string to parse! -h for usage\n");
		return 1;
	}
	if (strlen(name) == 0) {
		if (!quiet) fprintf(stderr, "-n should be the name of the variable you want to extract.\n");
		return 1;
	}
	if (name[strlen(name)-1] != '=') {
		if (!quiet) fprintf(stderr, "-n name should have an = at the end work properly\n");
	}

	/* find name in cgi string */
	cgi = argv[optind];
	if ((value = strstr(cgi,name)) == NULL) {
		return 2;
	}

	value += strlen(name);
	if (strlen(value)) {
		if ((end = strstr(value,"&")) == NULL) {
			cgiprintdecode(value,strlen(value));
		} else {
			cgiprintdecode(value,end - value);
		}
	}
	/* note that backticks remove the \n in the resulting string */
	printf("\n");
	return 0;
}

/**
 * decode a cgi value and put it in a destination string
 * as the destination string's memory is allocated by this function
 * make sure to free it after using the destination value.
 * @param value - input string to decode
 * @param len - number of bytes to decode
 * @return pointer to output string
 */
void cgiprintdecode(char *value, size_t len)
{
	int s;
	unsigned int c;

	/* decode will simply spit out verbatim %.. codes it does not understand eg %% */
	for (s=0; s < len; s++) {
		switch (value[s]) {
		case '+': 
			c = ' ';
		break;
		case '%': 
			if (sscanf(&value[s+1], "%2x", &c) == 1) {
				s += 2;
			} else {
				c = value[s];
			}
		break;
		default: c = value[s];
		}
		printf("%c", c);
	}
}

