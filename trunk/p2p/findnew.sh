#!/bin/sh
# run this in the p2p directory only
#
# see http://www.linuxquestions.org/questions/linux-newbie-8/how-to-loop-over-text-file-lines-within-bash-script-for-loop-522355/
source /etc/nap.conf
shared=$1/
idx=$napdata/index.txt
type=$2

if [ -f $idx ]
then
	list=`sed -e "s#[a-f0-9]* [0-9]* #\$shared#" $idx`
	find "$shared" -name "*.$type" | grep -v $napshared/links |\
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


