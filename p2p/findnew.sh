#!/bin/sh
shared=$1/
idx=$shared/index.txt
type=$2

if [ -f $idx ]
then
	list=`sed -e "s#[a-f0-9]* [0-9]* #\$shared#" $shared/index.txt`
	find "$shared" -name "*.$type" |\
	# see http://www.linuxquestions.org/questions/linux-newbie-8/how-to-loop-over-text-file-lines-within-bash-script-for-loop-522355/
	while read file
	do
		# seem to get messed up by [] characters in the file names when matching
		escaped=`echo "$file" | sed -e 's/\(\[\|\]\)/./g'`
		echo "$list" |\
		if [ `grep -c "$escaped"` = "0" ] 
		then 
			/p2p/indexline.sh "$file" "$shared"
		fi
	done
else
	echo no list
	find $shared -name "*.$type" -exec /p2p/indexline.sh {} $shared \;
fi


