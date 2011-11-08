all: napcmd napconn

ALCC=arm-linux-gcc
CC=gcc
OUTPATH=/opt/cmpt433/public/cwoodruf_nfs/bin

napcmd: napcmd.c
	-echo "building napcmd"
	$(CC) $(CFLAGS) napcmd.c -o napcmd-host
	$(ALCC) $(CFLAGS) napcmd.c -o napcmd 
	cp napcmd $(OUTPATH)

napconn: napconn.c napconn.h
	-echo "building napconn - not implemented yet"

