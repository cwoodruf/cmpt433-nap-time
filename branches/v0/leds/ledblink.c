/**
 * leddrv module test program
 * @author Cal Woodruff <cwoodruf@sfu.ca> 301013983
 */
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "leddrv.h"
#define LEDMASKMAX 15

int main(int argc, char **argv) {
	int mask = -1, count, timeout = -1, opt, i;

	while ((opt = getopt(argc,argv,"hm:t:")) != -1) {
		switch(opt) {
		case 'h':
			printf(
				"Usage: %s -m{mask} -t{timeout} [-h]\n"
				"\t-m{mask} numerical mask to show on leds must be >= 0\n"
				"\t-t{timeout} blink timeout - how long to wait in milliseconds between blinks (> 0)\n"
				"\t-h this help\n",
				argv[0]
			);
		return 0;
		case 'm': 
			mask = atoi(optarg);
		break;
		case 't':
			timeout = atoi(optarg);
		break;
		default:
			printf("Don't understand option %c. Try using -h for option list.\n",opt);
		return 1;
		}
	}
	if (mask < 0) {
		printf("mask is not set: should be >= 0. Try -h for help.\n");
		return 1;
	}
	count = mask;
	if (mask > LEDMASKMAX) mask = LEDMASKMAX;
	if (timeout <= 0) {
		printf("timeout is not set: should be > 0. Try -h for help.\n");
		return 1;
	}
	
	if (LEDDrv_init() < 0) {
		perror("failed to open device!");
		return 1;
	} 
	for (i=0; i<count; i++) {
		LEDDrv_setState(0);
		usleep(timeout*1000);
		LEDDrv_setState(mask);
		usleep(timeout*1000);
	}
	LEDDrv_setState(0);
	LEDDrv_cleanup();
	return 0;
}
