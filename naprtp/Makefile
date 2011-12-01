INC=-I${PUBLIC}/ortp/include -I${PUBLIC}/alsa/include
LIBS=-L${PUBLIC}/ortp/lib -L${PUBLIC}/alsa/lib -lspeex -lasound -lspeexdsp -lortp
CFLAGS+=-g -Wall -O0 -msoft-float -march=armv4t -mtune=arm920t -mcpu=arm920t
CPPFLAGS+=$(CFLAGS)
ARMCC=arm-linux-gcc
CC=gcc

naprtp: rtpsend.c rtprecv.c
	$(ARMCC) $(INC) $(LIBS) $(CFLAGS) rtpsend.c -o rtpsend-arm
	$(ARMCC) $(INC) $(LIBS) $(CFLAGS) rtprecv.c -o rtprecv-arm
	cp rtpsend-arm rtprecv-arm naprtpsend naprtprecv naprtpkill ${PUBLIC}/bin
	$(CC) -lspeex -lasound -lspeexdsp -lortp rtpsend.c -o rtpsend-x86
	$(CC) -lspeex -lasound -lspeexdsp -lortp rtprecv.c -o rtprecv-x86
