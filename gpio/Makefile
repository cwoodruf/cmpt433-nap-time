DEBUG=0
CC=arm-linux-gcc
CFLAGS=-g -Wall -DDEBUG=$(DEBUG) -march=armv4t -msoft-float -mtune=arm920t -mcpu=arm920t
OUTDIR=/opt/cmpt433/public/cwoodruf_nfs/bin

gpiodriver: gpiodriver.h gpiodriver.c gpiotest.c
	$(CC) $(CFLAGS) -c gpiodriver.c 
	$(CC) $(CFLAGS) gpiodriver.o gpiotest.c -o gpiotest
	cp gpiotest $(OUTDIR)

