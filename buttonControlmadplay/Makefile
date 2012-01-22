CC=arm-linux-gcc
EXECUTABLE=buttonControlmadplay
WRITER=writeFIFO
CFLAGS=-Wall -g

all: $(EXECUTABLE) $(WRITER)

$(EXECUTABLE):$(OBJS)
	$(CC) -o $(EXECUTABLE) $(EXECUTABLE).c
	-cp $(EXECUTABLE) ${PUBLIC}/bin

$(WRITER):$(OBJS)
	$(CC) $(CFLAGS) -o $(WRITER) $(WRITER).c
	-cp $(WRITER) ${PUBLIC}/bin

