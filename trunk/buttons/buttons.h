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
#define BUTTON_TOP 1
#define BUTTON_DOWN 2
#define BUTTON_LEFT 3
#define BUTTON_RIGHT 4
#define BUTTON_INVALID 5

struct ButtonState;

struct ButtonState read_buttons();
void initialize();
void cleanup ();
