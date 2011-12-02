/**
 * leddrv module test program
 * @author Cal Woodruff <cwoodruf@sfu.ca> 301013983
 */
#include <unistd.h>
#include <stdio.h>
#include "leddrv.h"

int main(int argc, char **argv) {
	int allleds = (1 << 3) | (1 << 2) | (1 << 1) | 1;
	printf("all leds: %d\n", allleds);
	if (LEDDrv_init() < 0) {
		perror("failed to open device!");
		return 1;
	} 
	printf("turning off all leds\n");
	LEDDrv_setState(0);
	LEDDrv_setState(allleds);
	printf("turned on all leds\n");
	sleep(2);
	printf("turning off all leds\n");
	LEDDrv_setState(0);
	LEDDrv_cleanup();
	return 0;
}
