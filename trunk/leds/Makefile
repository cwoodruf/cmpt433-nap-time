DEBUG=0
CC=arm-linux-gcc
CFLAGS=-g -Wall -DDEBUG=$(DEBUG) -march=armv4t -msoft-float -mtune=arm920t -mcpu=arm920t
OUTDIR=${PUBLIC}/bin

leddrv: leddrv.h leddrv.c ledtest.c ledblink.c
	$(CC) $(CFLAGS) -c leddrv.c 
	$(CC) $(CFLAGS) leddrv.o ledtest.c -o ledtest
	$(CC) $(CFLAGS) leddrv.o ledblink.c -o ledblink
	if [ -d "$(OUTDIR)" ] ; then cp ledtest ledblink $(OUTDIR) ; fi

