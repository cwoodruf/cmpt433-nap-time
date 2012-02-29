#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>
#include "gpiodriver.h"

static int gh = -1;

/**
 * connect to the gpio device
 */
int gpio_init()
{
	if ((gh = open(GPIODEV,O_RDWR)) < 0) {
		perror("can't open gpio device");
		gh = -1;
		return -1;
	}
	return gh;
}

/**
 * write to a writeable pin
 */
void gpio_write(int pin)
{
	if (gh == -1) {
		perror("initialize device with gpio_init() first!");
		return;
	}
	ioctl(gh,pin);
}

/**
 * read from a readable pin via interrupt
 * this should be in its own thread so it can wait
 * for input from any of the pins - returns which pin 
 * this doesn't block
 */
char gpio_read()
{
	fd_set rds;
	int ret,i,rd;
	char mask;
	unsigned char pins[GPIOPINS];

	if (gh == -1) {
		perror("initialize device with gpio_init() first!");
		return -1;
	}

	FD_ZERO(&rds);
	FD_SET(gh, &rds);

	ret = select(gh+1, &rds, NULL, NULL, NULL);
	if (ret < 0) {
		perror("gpiodriver: error listening for gpio input!");
		return -1;

	} else if (ret == 0) {
		perror("gpiodriver: timeout ... (shouldn't be here!)");
		return -2;

	} else if (FD_ISSET(gh, &rds)) {
		rd = read(gh,pins,sizeof(pins));
		if (rd < sizeof(pins) && errno != EAGAIN) {
			perror("read error");
			return -1;
		} 
		mask = 0;
		for (i=0; i<GPIOPINS; i++) {
			if (pins[i] == '1') {
				mask |= (1 << i);
			}
		}
	}
	return -3;
}

/**
 * close the device
 */
void gpio_cleanup()
{
	close(gh);
}

