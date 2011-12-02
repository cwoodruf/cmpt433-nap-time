/**
 * cmpt433 nap time systems
 * just buzz the buzzer for a predetermined amount of time - or not
 * @author Cal Woodruff <cwoodruf@sfu.ca> 301013983
 */

#include <stdio.h>
#include <unistd.h>
#include "buzzer.h"
// #define BUZZER_Debug

int main(int argc, char **argv) 
{
	useconds_t duration;
	int freq, ret;

	if (buzzer_init()) return 1;

	if (argc == 3) {
		sscanf(argv[1], "%d", &freq);
		sscanf(argv[2], "%ul", &duration);
		printf("playing buzzer: freq %d duration %u usecs\n", freq, duration);
		ret = buzzer_buzz(duration,freq);

	} else if (argc == 2) {
		sscanf(argv[1], "%d", &freq);
		printf("playing buzzer: freq %d (0 shuts off buzzer)\n", freq);
		if (freq <= 0) ret = buzzer_off();
		else ret = buzzer_on(freq);

	} else {
		printf("usage: %s {freq} [duration]\n\tfreq in hz\n\tduration optional in usecs\n",argv[0]);
		ret = 3;
	}

	buzzer_cleanup();
	return ret;
}

