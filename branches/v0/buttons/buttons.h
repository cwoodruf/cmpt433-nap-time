#ifndef BUTTONS_H_
#define BUTTONS_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/select.h>
#include <sys/time.h>
#include <errno.h>

#define STATE_PRESS 1
#define STATE_RELEASE 0
#define BUTTON_INVALID 4

struct ButtonState {
	int code;
	int state;
};

struct ButtonState read_buttons();
void initialize();
void cleanup ();

#endif //BUTTONS_H_
