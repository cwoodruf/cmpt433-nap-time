#!/bin/sh
sha=`sha1sum "$1" | sed -e 's/^\([a-f0-9]*\).*/\1/'`
size=`stat -c"%s" "$1"`
path=`./canonicalize "$1"`
echo $sha $size $path
