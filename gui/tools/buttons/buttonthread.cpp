/**
 * cmpt433 assignment 4 Cal Woodruff <cwoodruf@sfu.ca> 301013983
 * based on Brian Fraser's example
 * found here:
 * http://www2.cs.sfu.ca/CourseCentral/433/bfraser/solutions/qtthread/buttonthread.zip
 * using code from assignment 3 for the button interface
 */
#include "buttonthread.h"

extern "C" {
	#include <stdio.h>
	#include "buttons.h"
}

ButtonThread::ButtonThread(void)
{
}

/**
 * function that gets run when we start the thread interface
 * via the ->start call
 * initialize buttons and then loop forever waiting for any
 * activity send the button mask along when we get something
 */
void ButtonThread::run(void)
{
	int buttonmask;

	if (buttons_init() != 0) return;
	while (true) {
		buttonmask = buttons_waitpress(BUTTON_ANY);	
		if (buttonmask > 0) emit buttonPressed(buttonmask);
	}
}

