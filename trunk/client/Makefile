all: napcmd napconn

ALCC=arm-linux-gcc
CC=gcc
OUTPATH=/opt/cmpt433/public/cwoodruf_nfs/bin
CFLAGS+=-Wall -g -lm -pthread
ALCFLAGS+=-march=armv4t -msoft-float -mtune=arm920t -mcpu=arm920t

napcmd: napcmd.c nap.h
	-echo "building napcmd"
	$(CC) $(CFLAGS) napcmd.c -o napcmd-host
	$(ALCC) $(CFLAGS) napcmd.c -o napcmd $(ALCFLAGS)
	cp napcmd $(OUTPATH)

napconn: napconn.c napconn.h nap.h napfile.h napfile.c
	-echo "building napconn"
	$(CC) $(CFLAGS) napconn.c napfile.c -o napconn-host
	$(ALCC) $(CFLAGS) napconn.c napfile.c -o napconn $(ALCFLAGS)
	cp napconn $(OUTPATH)

