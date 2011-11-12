#!/bin/sh
# run this in the p2p directory
# see http://www.linuxquestions.org/questions/linux-newbie-8/how-to-loop-over-text-file-lines-within-bash-script-for-loop-522355/
shared=$1/
idx=$shared/index.txt
type=$2

if [ -f $idx ]
then
	list=`sed -e "s#[a-f0-9]* [0-9]* #\$shared#" $shared/index.txt`
	find "$shared" -name "*.$type" | grep -v shared/links |\
	while read file
	do
		# seem to get messed up by [] characters in the file names when matching
		escaped=`echo "$file" | sed -e 's/\(\[\|\]\)/./g'`
		echo "$list" |\
		if [ `grep -c "$escaped"` = "0" ] 
		then 
			./indexline.sh "$file" 
		fi
	done
else
	find $shared -name "*.$type" -exec ./indexline.sh {} \;
fi


