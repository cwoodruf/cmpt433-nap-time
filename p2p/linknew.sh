#!/bin/sh
cat shared/index.txt |\
while read l
do
	ext=`echo "$l" | sed -e 's/.*\.//'`
	sum=`echo "$l" | sed -e 's/ .*//'`
	file=`echo "$l" | sed -e 's/^[a-f0-9]* [0-9]* //'`
	link=shared/links/$sum.$ext
	if [ -f $link ] 
	then
		rm $link
	fi
	path=`./canonicalize "$1/$file"`
	ln -s "$path" $link
done
