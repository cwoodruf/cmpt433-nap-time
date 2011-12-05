/**
 * buttons module for cmpt433 assignment 3
 * based on the buttons EmbedSky sample application
 * @author Cal Woodruff <cwoodruf@sfu.ca> 301013983
 */
#include <stdio.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/select.h>
#include <sys/time.h>
#include <errno.h>
#include <assert.h>
#include "buttons.h"

#define BUTTONS_DEV "/dev/IRQ-Test"
/* how long to wait in usecs if we are not blocking */
#define BUTTONS_UWAIT 100000

#ifndef BUTTONS_Debug
#define BUTTONS_Debug DEBUG
#endif

static int buttons_fd = -1;

/* get what buttons were pressed */
static int _buttons_extract(void);

/**
 * open buttons device
 * returns 0 on success
 */
int buttons_init(void)
{
	buttons_fd = open(BUTTONS_DEV, 0);
	if (buttons_fd < 0)
	{
#if BUTTONS_Debug
		perror("buttons: error opening buttons device!");
#endif
		return -1;
	}
	return 0;
}

/**
 * close the buttons device
 */
void buttons_cleanup(void) 
{
	close(buttons_fd);
}

/**
 * get which button was pressed from a mask
 * state can be -1 for released, 1 for pressed and 0 for any
 */
int button_get(int mask)
{
	int i;
	for (i = 0; i<BUTTON_COUNT; i++) {
		if (BUTTON_PRESSED(mask,i)) return i;
	}
	return -1;
}

/**
 * check whether one or more buttons were pressed
 * this function does not block
 * returns a mask of which buttons were pressed or a negative value on error
 * the lower nibble bit is set to indicate the button is being pressed
 */
int buttons_pressed(long wait)
{
	fd_set rds;
	int ret;
	struct timeval tv;

	assert(buttons_fd >= 0);

	FD_ZERO(&rds);
	FD_SET(buttons_fd, &rds);
	if (wait > 0) {
		tv.tv_sec = 0;
		tv.tv_usec = wait;
		ret = select(buttons_fd+1, &rds, NULL, NULL, &tv);
	} else {
		ret = select(buttons_fd+1, &rds, NULL, NULL, NULL);
	}
	if (ret < 0) {
#if BUTTONS_Debug
		perror("buttons: error listening for button press!");
#endif
		return -1;
	} else if (ret == 0) {
#if BUTTONS_Debug
		perror("buttons: timeout ...");
#endif
		return -2;
	} else if (FD_ISSET(buttons_fd, &rds)) {
		return _buttons_extract();
	}
	return -3;
}

/**
 * block until a specific button is pressed
 * or use BUTTON_ANY and read returned mask
 * input of BUTTON_RELEASED waits until all buttons are released
 */
int buttons_waitpress(int button)
{
	fd_set rds;
	int ret, mask;

	assert(buttons_fd >= 0);

	FD_ZERO(&rds);
	FD_SET(buttons_fd, &rds);
	while (1) {
		ret = select(buttons_fd+1, &rds, NULL, NULL, NULL);
		if (ret < 0) {
#if BUTTONS_Debug
			perror("buttons: error listening for button press!");
#endif
			return -1;
		} else if (ret == 0) {
#if BUTTONS_Debug
			perror("buttons: timeout waiting for buttons!");
#endif
			return -2;
		} else if (FD_ISSET(buttons_fd, &rds)) {
			mask = _buttons_extract();
			/* for buttons_waitrelease */
			if (button == BUTTON_RELEASED && !mask) return 0;
			/* normal usage */
			if ((button == BUTTON_ANY) || BUTTON_PRESSED(mask,button)) return mask;
		}
	}
	return 0;
}

/**
 * block until buttons are all released 
 * convenience function that runs buttons_waitpress(BUTTON_RELEASED)
 */
int buttons_waitrelease(void) {
	int ret = buttons_waitpress(BUTTON_RELEASED);
	return (ret == 0 ? 1 : 0);
}

/**
 * return what buttons were pressed as a mask
 */
static int _buttons_extract(void) 
{
	unsigned char keystate[BUTTON_COUNT];
	int ret, i, mask = 0;

	ret = read(buttons_fd, keystate, sizeof(keystate));
	/* we have not read the state of all buttons */
	if (ret != sizeof(keystate))
	{
		if (errno != EAGAIN) {
#if BUTTONS_Debug
			perror("buttons: read error!");
#endif
			return -1;
		}
	} else {
		for (i = 0; i < BUTTON_COUNT; i++) {
			/* they actually use the "0" and "1" characters */
			if(keystate[i] == '1') {
				mask |= (1 << i);
			}
#if BUTTONS_Debug
			printf("%c for button %d = mask %d\n", keystate[i], i, mask);
#endif
		}
	}
	return mask;
}

/* end buttons.c */

