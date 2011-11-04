#!/bin/sh
source /etc/p2p.conf
echo "content-type: text/plain"
echo
pw=`echo "$QUERY_STRING" | grep -o '=[A-Za-z0-9]*' | sed -e 's/^=//'`
if [ $pw == $PASSWORD ] 
then
	/bin/touch "$IPDIR/$REMOTE_ADDR" && echo OK && return
	echo FAIL:touch
else
	echo FAIL
fi

