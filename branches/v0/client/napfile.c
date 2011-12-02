/**
 * cmpt433 final project nap time systems
 * from Tom Betz
 * references?
 */
#include <stdio.h>
#include <unistd.h>

void nap_chomp (char* buffer);

int nap_readFile (char* path, char* buffer, int count) {
	FILE* file = fopen (path, "r");
	if (file == NULL) {
		return -1;
	}
	char* result = fgets (buffer, count, file);
	if (result == NULL) {
		fclose (file);
		return -1;
	}

	fclose (file);
	nap_chomp (buffer);
	return 0;
}

void nap_chomp (char* buffer) {
	char* p = buffer;
	if (p == NULL) {
		return;
	}
	while ((*p) != '\0') {
		if ((*p) == '\n') {
			break;
		}
		p++;
	}
	(*p) = 0;
}

int nap_fileExists (char* path) {
	if (access (path, F_OK) == 0) {
		return 1;
	}
	return 0;
}
