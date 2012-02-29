#ifndef GPIODRIVER
#define GPIODRIVER

#define GPIODEV "/dev/GPIO-other-pins"
#define GPIOPINS 8

/**
 * connect to the gpio device
 */
int gpio_init();

/**
 * write to a writeable pin
 */
void gpio_write(int pin);

/**
 * read from a readable pin via interrupt
 * this should be in its own thread so it can wait
 * for input from any of the pins - returns which pin was read
 */
char gpio_read();

/**
 * close the device
 */
void gpio_cleanup();

#endif
