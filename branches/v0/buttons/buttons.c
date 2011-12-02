#define MASK_BIT_1	1

#include "buttons.h"


static int ButtonsState[]={0,0,0,0};	
int buttons_fd;

struct ButtonState read_buttons()
{
	int i;
	char key_value[4];
	struct ButtonState buttonState = { BUTTON_INVALID, 0 };
	fd_set rds;
	int ret;

	FD_ZERO(&rds);
	FD_SET(buttons_fd, &rds);
	ret = select(buttons_fd + 1, &rds, NULL, NULL, NULL);
	if (ret < 0)
	{
		perror("select");
	}
	else if (ret == 0)
	{
		printf("Timeout.\n");
	} 
	else if (FD_ISSET(buttons_fd, &rds))
	{
		ret = read(buttons_fd, key_value, sizeof(key_value));
		if (ret != sizeof(key_value))
		{
			if (errno != EAGAIN)
				perror("read buttons failed\n");
		}
		else
		{
			for (i = 0; i < 4; i++) {
				if(key_value[i] != 0)
				{
					if (key_value[i] == 49 && ButtonsState[i] != 1) {
						buttonState.code = i;
						buttonState.state = STATE_PRESS;
						ButtonsState[i]=1;
						//printf ("Press %s\n", ButtonNames[i]);
					} 
					else if (key_value[i] == 48 && ButtonsState[i] != 0) {
						buttonState.code = i;
						buttonState.state = STATE_RELEASE;
						ButtonsState[i]=0;
						//printf ("Release %s\n", ButtonNames[i]);
					}
				}
			}
		}
	}

	return buttonState;
}

void initialize()
{
	buttons_fd = open("/dev/IRQ-Test", 0);
	if (buttons_fd < 0)
	{
		perror("buttons open failed");
		exit(1);
	}
	read_buttons ();
}


void cleanup () 
{
	close(buttons_fd);
}
