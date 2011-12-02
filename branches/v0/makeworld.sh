#!/bin/sh
# builds components for the naptime media player for the cmpt433 final project fall 2011
# you'll want to define where the output goes in a PUBLIC shell variable
# export PUBLIC=../public
# also you will need external libraries for the naprtp target to build specifically oRTP, speex and alsa
# see our how to guide (separate) for instructions on how to make these components
# you will want to change the LIB defined in the Makefile to point where these reside
# they are assumed to all be in the same subdirectory

make -B 

