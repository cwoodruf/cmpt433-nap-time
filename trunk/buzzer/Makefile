DEBUG=0
CC=arm-linux-gcc
CFLAGS=-g -Wall -DDEBUG=$(DEBUG) -march=armv4t -msoft-float -mtune=arm920t -mcpu=arm920t
OUTDIR=${PUBLIC}/bin

buzzer: buzzer.h buzzer.c buzz.c
	$(CC) $(CFLAGS) -c buzzer.c
	$(CC) $(CFLAGS) buzzer.o buzz.c -o buzz
	cp buzz $(OUTDIR)


