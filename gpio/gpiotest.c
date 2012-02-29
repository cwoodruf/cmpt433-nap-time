#include <stdio.h>
#include "gpiodriver.h"

int main(int argc, char **argv)
{
	if (gpio_init() < 0) {
		return -1;
	}
	gpio_cleanup();
	return 0;
}
