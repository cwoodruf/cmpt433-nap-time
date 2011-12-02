/**
 * buttons module header file for cmpt433 assignment 3
 * @author Cal Woodruff <cwoodruf@sfu.ca> 301013983
 * 
 * EmbedSky_irq.ko compilation note:
 *
 * you must set linux make menuconfig (device drivers > character devices) 

       <M> EmbedSky TQ2440 Board IRQ Test(Buttons test) 
         [ ]   EmbedSky SKY2440 Board IRQ Test(Buttons test)          

 * for the up and down buttons to be detectable
 * *ing the second sets a flag that changes which interrupts are mapped to the buttons
 * and the left and right buttons will be the only ones that work
 */

#ifndef BUTTONS_H_
#define BUTTONS_H_

#define BUTTON_UP	0
#define BUTTON_DOWN	1
#define BUTTON_LEFT 	2
#define BUTTON_RIGHT	3
#define BUTTON_ANY	-1
#define BUTTON_RELEASED	-2

#define BUTTON_COUNT	4

/* compares mask m with button b to see the button state */
#define BUTTON_PRESSED(m,b)	((m & (1 << b)) ? 1 : 0)

/**
 * open buttons device
 * returns 0 on success -1 on failure
 */
int buttons_init(void);

/**
 * close the buttons device
 */
void buttons_cleanup(void);

/**
 * check whether one or more buttons were pressed
 * this function does not block - it will wait for the given wait time instead
 * returns a mask of which buttons were pressed or a negative value on error
 * the lower nibble bit is set to indicate the button
 * note that you may need to use this twice initially and discard the first return value
 */
int buttons_pressed(long wait);

/**
 * block until a specific button is pressed
 * input of BUTTON_ANY returns if any button is pressed
 * input of BUTTON_RELEASED returns 1 if buttons are all released
 * returns mask of buttons pressed
 */
int buttons_waitpress(int button);

/**
 * block until buttons are all released 
 * convenience function that runs buttons_waitpress(BUTTON_RELEASED)
 */
int buttons_waitrelease(void);

/**
 * get which button was pressed from a mask
 * returns < 0 if no button was pressed
 */
int button_get(int mask);

#endif /* BUTTONS_H_ */

