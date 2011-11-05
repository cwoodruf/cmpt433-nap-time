CC=arm-linux-gcc
CFLAGS=-Wall -g -march=armv4t -msoft-float -mtune=arm920t -mcpu=arm920t
PUBLICPATH=~/cmpt433/public/
PUBLICUSBAUTOMOUNT=$(PUBLICPATH)$(EXECUTABLE)

EXECUTABLE=usbAutoMount

SRCS:=$(wildcard *.c)
OBJS:=$(patsubst %.c, %.o, $(wildcard *.c))

all:$(EXECUTABLE)

$(EXECUTABLE): $(OBJS)
	$(CC) $(CFLAGS) -o $(EXECUTABLE) $(OBJS)
	-rm $(PUBLICUSBAUTOMOUNT)
	cp $(EXECUTABLE) $(PUBLICPATH)


clean:
	-rm $(PUBLICUSBAUTOMOUNT) $(EXECUTABLE)
