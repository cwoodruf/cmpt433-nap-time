all: musicScanServer

CC=g++
ALCC=arm-linux-g++
CFLAGS+=-Wall -g -lm
ALCFLAGS=$(CFLAGS) -march=armv4t -msoft-float -mtune=arm920t -mcpu=arm920t
OUTDIR=${HOME}/cmpt433/public/bin

musicScanServer: musicScanServer.cpp
	$(CC) $(CFLAGS) musicScanServer.cpp -o musicScanServer-host
	$(ALCC) $(ALCFLAGS) musicScanServer.cpp -o musicScanServer-arm
	cp musicScanServer-arm $(OUTDIR)
