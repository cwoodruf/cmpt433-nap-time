all: napcmd napconn

ALCC=arm-linux-gcc
CC=gcc
OUTPATH=/opt/cmpt433/public/cwoodruf_nfs/bin

napcmd: napcmd.c
	$(CC) $(CFLAGS) -o napcmd-host
	$(ALCC) $(CFLAGS) -o napcmd 
	cp napcmd $(OUTPATH)

napconn: napconn.c napconn.h
	-echo "nothing to do!"

