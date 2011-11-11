all: cginames cgiparser

cginames: cginames.c
	gcc -g cginames.c -o cginames-host
	arm-linux-gcc -g cginames.c -o cginames
	cp cginames ${HOME}/cmpt433/public/bin

cgiparser: cgiparser.c
	gcc -g cgiparser.c -o cgiparser-host
	arm-linux-gcc -g cgiparser.c -o cgiparser
	cp cgiparser ${HOME}/cmpt433/public/bin
