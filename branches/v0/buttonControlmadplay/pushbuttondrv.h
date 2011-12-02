/*
 * pushbuttondrv.h
 * Module to drive the Push Buttons.
 *
 *  Created on: Oct 12, 2011
 *      Author: Benny Yan
 */

#ifndef PUSHBUTTONDRV_H_
#define PUSHBUTTONDRV_H_

/*
 * Button Index in read function.
 */

#define RIGHT_BUTTON    3
#define LEFT_BUTTON	2
#define DOWN_BUTTON     1
#define UP_BUTTON    	0
#define TIME_OUT	-1

#define RIGHT_BUTTON_REP 1
#define DOWN_BUTTON_REP  (1<<RIGHT_BUTTON_REP)
#define UP_BUTTON_REP    (1<<DOWN_BUTTON_REP)
#define LEFT_BUTTON_REP  (2<<DOWN_BUTTON_REP)

/*
 * Initialize the Push Button Drivers
 * Returns 0 for success.
 */
int PUSHBUTTONDrv_init();

/*
 * Done with using the Push Button module:
 */
void PUSHBUTTONDrv_cleanup();

/*
 * Detect the button press
 * Returns 0 for success.
 */
int PUSHBUTTONDrv_detect();


#endif /* PUSHBUTTONDRV_H_ */
