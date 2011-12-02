# change this directory to where you want your code to ultimately reside
# this directory should be visible on the board as a mount point
# you will want to modify your PATH variable on the board to point to the
# bin and p2p directories
# you will also want to define a PUBLIC directory and make bin directory in there
LIB=../public

all: p2p buttonControlmadplay cgiparser leds musicscan napscan usbAutoMount buzzer intercom launcher memos player madplay naprtp

# this assumes the ${PUBLIC} directory does not have anything in it
p2p: 
	@(mkdir -p ${PUBLIC}/bin; mkdir -p ${PUBLIC}/p2p/shared/memos; chmod a+rwx ${PUBLIC}/p2p/shared/memos; cp -r p2p/* ${PUBLIC}/p2p; cp gui/scripts/* ${PUBLIC}/bin)

buttonControlmadplay:
	make --directory=buttonControlmadplay

cgiparser:
	make --directory=cgiparser

leds:
	make --directory=leds

musicscan:
	make --directory=musicscan

napscan:
	make --directory=napscan

usbAutoMount:
	make --directory=usbAutoMount

buzzer:
	make --directory=buzzer

intercom:
	(cd gui/intercom; ./make.sh)

launcher:
	(cd gui/launcher; ./make.sh)

memos:
	(cd gui/memos; ./make.sh)

player:
	(cd gui/player; ./make.sh)

madplay:
	(cd madplay-0.15.2b; cp madplay ${PUBLIC}/bin)

naprtp: 
	make --directory=naprtp LIB=$(LIB)

