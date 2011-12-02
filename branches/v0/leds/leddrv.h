/*
 * leddrv.h
 * Module to drive the LEDs.
 *
 *  Created on: Oct 5, 2011
 *      Author: Brian Fraser
 */

#ifndef LEDDRV_H_
#define LEDDRV_H_

/*
 * LED bit positions.
 */
#define LED_BIT_0    0
#define LED_BIT_1    1
#define LED_BIT_2    2
#define LED_BIT_3    3

#define LED_NUM_LEDS 4

/*
 * LED bit polarity
 */
#define LED_ON	1
#define LED_OFF	0

/*
 * Initialize the LEDs
 * Returns 0 for success.
 */
int LEDDrv_init();

/*
 * Done with using the LED module:
 */
void LEDDrv_cleanup();

/*
 * Set the state of the buttons based based on the bit values in mask.
 * Returns 0 for success.
 */
int LEDDrv_setState(int mask);


#endif /* LEDDRV_H_ */
