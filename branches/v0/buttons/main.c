#include <stdio.h>
#include "buttons.h"

int main () {
	char* buttonNames[] = { "UP", "DOWN", "LEFT", "RIGHT", "INVALID" };
	initialize ();
	struct ButtonState buttonState;
	buttonState = read_buttons ();
	printf ("%s, %s\n", buttonNames[buttonState.code], ((buttonState.state == 1) ? "PRESSED" : "RELEASED"));
	buttonState = read_buttons ();
	printf ("%s, %s\n", buttonNames[buttonState.code], ((buttonState.state == 1) ? "PRESSED" : "RELEASED"));
	cleanup ();

	return 0;
}
