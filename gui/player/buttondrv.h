/*
 * buttondrv.h
 * Module to read the buttons.
 *
 *  Created on: Oct 5, 2011
 *      Author: Brian Fraser
 */

#ifndef BUTTONDRV_H_
#define BUTTONDRV_H_


#define BTN_NONE	0x00

// Button bit numbers
#define BTN_BIT_UP		0
#define BTN_BIT_LEFT	2
#define BTN_BIT_RIGHT	3
#define BTN_BIT_DOWN	1

/*
 * Initialize the Buttons
 * Returns 0 for success.
 */
int ButtonDrv_init();

/*
 * Done with using the buttons module:
 */
void ButtonDrv_cleanup();

/*
 * Reads the state of the buttons.
 * - Changes pbtnPressMsk to be a bitmask of the buttons which are pressed.
 * - Returns 0 for success.
 */
int ButtonDrv_read(int *pbtnPressMsk);

#endif /* BUTTONDRV_H_ */
