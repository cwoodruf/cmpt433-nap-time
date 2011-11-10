all: napsender naplistener

CC=gcc
ALCC=arm-linux-gcc
CFLAGS+=-Wall -g -lm -pg
ALCFLAGS=$(CFLAGS) -march=armv4t -msoft-float -mtune=arm920t -mcpu=arm920t
OUTDIR=/opt/cmpt433/public/cwoodruf_nfs/bin

napsender: napsender.c napmulticast.h
	$(CC) $(CFLAGS) napmulticast.h napsender.c -o napsender-host
	$(ALCC) $(ALCFLAGS) napsender.c -o napsender
	cp napsender $(OUTDIR)

naplistener: naplistener.c napmulticast.h
	$(CC) $(CFLAGS) naplistener.c -o naplistener-host
	$(ALCC) $(ALCFLAGS) naplistener.c -o naplistener
	cp naplistener $(OUTDIR)

