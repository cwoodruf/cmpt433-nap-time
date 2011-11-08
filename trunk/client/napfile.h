/**
 * cmpt433 final project nap time systems
 * from Tom Betz
 */
#ifndef NAPFILE
#define NAPFILE
/**
* read file content.
* path: path to the file
* buffer: buffer to store the content read
* count: maximum bytes to be read
* return -1 upon failure, 0 upon success
**/
int nap_readFile (char* path, char* buffer, int count);

/**
* return 1 if file exists
* 0 if file doesn't exist
**/
int nap_fileExists (char* path);
/**
 * remove line ending character
 */
void nap_chomp(char *str);

#endif /* NAPFILE */

