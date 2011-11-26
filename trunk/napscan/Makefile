all: napping naplistener

CC=gcc
ALCC=arm-linux-gcc
CFLAGS+=-Wall -g -lm
ALCFLAGS=$(CFLAGS) -march=armv4t -msoft-float -mtune=arm920t -mcpu=arm920t
OUTDIR=${HOME}/cmpt433/public/bin

napping: napping.c napmulticast.h
	$(CC) $(CFLAGS) napmulticast.h napping.c -o napping-host
	$(ALCC) $(ALCFLAGS) napping.c -o napping
	cp napping $(OUTDIR)

naplistener: naplistener.c napmulticast.h buzzer.c buzzer.h
	$(CC) $(CFLAGS) naplistener.c -o naplistener-host
	$(ALCC) $(ALCFLAGS) naplistener.c buzzer.c -DIS_EM2440=1 -o naplistener
	cp naplistener $(OUTDIR)

