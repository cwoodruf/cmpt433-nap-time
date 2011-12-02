#!/bin/sh
# builds components for the naptime media player for the cmpt433 final project fall 2011
# you'll want to define where the output goes in a PUBLIC shell variable
# export PUBLIC=../public
# also you will need external libraries for the naprtp target to build specifically oRTP, speex and alsa
# see our how to guide (separate) for instructions on how to make these components
# you will want to change the LIB defined in the Makefile to point where these reside
# they are assumed to all be in the same subdirectory
usage="Usage: $0 {directory with alsa ortp and speex}"

if [ "$PUBLIC" = "" ]
then
	echo You need to define a shell variable PUBLIC to a directory for the output of the make
	echo $usage
	return
fi
if [ "$1" = "" ] || [ ! -d "$1/alsa" ] || [ ! -d "$1/ortp" ] 
then
	echo You need our precompiled libraries to build the naprtp component
	echo $usage
	echo to run anyway run make -B instead of this script
	return
fi

make -B LIB="$1"

