#!/bin/sh
if [ "$1" = "" ]
then
	echo usage: $0 {target}
	return
fi
arm-linux-qmake -o Makefile.arm DEFINES+=EM2440
make -B -f Makefile.arm 
mv $1 $1-arm
if [ "$PUBLIC" != "" ] ; then 
	if [ -d "$PUBLIC/bin" ] ; then echo; cp -v $1-arm $PUBLIC/bin ; echo; fi ; 
fi
qmake -o Makefile.x86
make -B -f Makefile.x86
mv $1 $1-x86 

