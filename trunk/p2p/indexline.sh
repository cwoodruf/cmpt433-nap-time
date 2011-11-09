#!/bin/sh
sha=`sha1sum "$1" | sed -e 's/^\([a-f0-9]*\).*/\1/`
size=`stat -c"%s" "$1"`
if [ $2 ] ; 
then
	file=`echo "$1" | sed -e "s#^\$2##"`
else 
	file=$1
fi
echo $sha $size $file
