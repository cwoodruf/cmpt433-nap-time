/*
 * button.c
 * Module to read the buttons.
 *
 *  Created on: Oct 5, 2011
 *      Author: Brian Fraser; based on code by EmbedSky (button.c)
 */

#include "buttondrv.h"

#include <stdio.h>
#include <stdlib.h>
#include <linux/stddef.h>		// for NULL
#include <fcntl.h>		// for open()
#include <unistd.h>		// for close()
#include <sys/ioctl.h>	// for ioctl()
#include <sys/select.h>
#include <errno.h>		// for errno
#include <stdbool.h>	// for true/false

#define ButtonDrv_DEVICE_FILE "/dev/IRQ-Test"
static int ButtonDrv_fd = -1;

int ButtonDrv_init()
{
	ButtonDrv_fd = open(ButtonDrv_DEVICE_FILE, 0);
	if (ButtonDrv_fd < 0) {
		fprintf(stderr, "Can't open button device (%s).\n", ButtonDrv_DEVICE_FILE);
		return(-1);
	}
	return 0;
}

/*
 * Done with using the buttons module:
 */
void ButtonDrv_cleanup()
{
	close(ButtonDrv_fd);
}

int ButtonDrv_read(int *pbuttonsPressed)
{
	int buttonState = 0;
	while (1) {
		// Setup the set of fd to watch:
		fd_set rds;
		FD_ZERO(&rds);
		FD_SET(ButtonDrv_fd, &rds);

		// Watch fd for readability:
		// Parameters:
		//		nfds: Highest # FD in any description, +1.
		//		fd to read: the button FD (data available)
		//		fd for writing: NULL (none)
		// 		fd for exceptions: NULL (none)
		//		timeout: NULL = wait indefinitely.
		int ret = select(ButtonDrv_fd + 1, &rds, NULL, NULL, NULL);
		if (ret < 0) {
			fprintf(stderr, "Select failure: %d.\n", ret);
			return(-1);
		}

		if (ret == 0) {
			printf("Select timed out.\n");
		}

		// Check which FDs are ready
		else if (FD_ISSET(ButtonDrv_fd, &rds)) {
			// Button is ready, read in data.
			char key_value[4];
			int ret = read(ButtonDrv_fd, key_value, sizeof(key_value));
			if (ret != sizeof(key_value)) {
				if (errno != EAGAIN)
					fprintf(stderr, "Read button error: error=%d, val=%d, sive=%d.\n",
							errno, 0, ret);
				continue;
			} else {
				int i;
				for (i = 0; i < 4; i++) {
					buttonState |= (key_value[i] != '0') << i;
					//if(key_value[i] != 0)
					//	printf("Key %d = 0x%02x\n",	i, key_value[i]);
				}
				// Data's good!
				break;

			}
		}
	}

	// Success
	*pbuttonsPressed = buttonState;
	return 0;
}
