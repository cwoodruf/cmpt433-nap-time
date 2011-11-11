#!/bin/sh
./findnew.sh $1 $2 |\
while read l
do
	sum=`echo "$l" | sed -e 's/ .*//'`
	file=`echo "$l" | sed -e 's/^[a-f0-9]* [0-9]* //'`
	if [ ! -f shared/links/$sum ] 
	then
		path=`./canonicalize "$1/$file"`
		ln -s "$path" shared/links/$sum
		ln -s "$path" shared/links/$sum.$2
		echo $l
	fi
done
