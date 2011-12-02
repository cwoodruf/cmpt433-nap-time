/**
 * Implementation of LED driver for cmpt433 assignment 3
 * @author Cal Woodruff <cwoodruf@sfu.ca> 301013983
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <assert.h>
#include "leddrv.h"

#define LED_DEVICE "/dev/GPIO-Control"

#ifndef LEDDrv_DEBUG
#define LEDDrv_DEBUG DEBUG
#endif

static int LEDDrv_fd = -1;

/*
 * Initialize the LEDs
 * Returns 0 for success.
 */
int LEDDrv_init() 
{
	LEDDrv_fd = open(LED_DEVICE, 0);
	if (LEDDrv_fd < 0) {
#if LEDDrv_DEBUG
		perror("LEDDrv: error opening led device");
#endif
		return 1;
	}
	return 0;
}

/*
 * Done with using the LED module:
 */
void LEDDrv_cleanup() 
{
        close(LEDDrv_fd);
}

/*
 * Set the state of the buttons based based on the bit values in mask.
 * Returns 0 for success.
 */
int LEDDrv_setState(int mask) 
{
	int led, state;

	assert(LEDDrv_fd >= 0);

#if LEDDrv_DEBUG
	printf("mask: %d\n", mask);
#endif
	for (led=0; led<LED_NUM_LEDS; led++) {
		state = ((mask & (1 << led)) >> led);
#if LEDDrv_DEBUG
		printf("state for led %d is %d\n", led, state);
#endif
		if ((ioctl(LEDDrv_fd, state, led)) == -1) {
#if LEDDrv_DEBUG
			perror("LEDDrv: error changing led");
#endif
			return 1;
		}
	}
	return 0;
}

/* end leddrv.c */

