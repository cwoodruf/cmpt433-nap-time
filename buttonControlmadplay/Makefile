CC=arm-linux-gcc
EXECUTABLE=buttonControlmadplay
CFLAGS=-Wall -g
COMPILE=$(CC) $(CFLAGS) -c 

SRCS:=$(wildcard *.c)
OBJS:=$(patsubst %.c, %.o, $(SRCS))

$(EXECUTABLE):$(OBJS)
	$(CC) -o $(EXECUTABLE) $(OBJS)
	-cp $(EXECUTABLE) /home/benny/cmpt433/public/

%.o: %.c
	$(COMPILE) -o $@ $< 
