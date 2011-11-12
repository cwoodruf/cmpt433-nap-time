#include <stdio.h>
#include "buttons.h"

void main () {
	initialize ();
	struct ButtonState buttonState = read_buttons ();
	printf ("%d, %d\n", buttonState.name, buttonState.state);
	cleanup ();
}
