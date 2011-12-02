#!/bin/sh
# get the connection parameters and ask the end user for confirmation from the gui
. /etc/nap.conf

ip=$REMOTE_ADDR
sendport=`param sendport | sed -e 's/[^0-9]*//g'`
recvport=`param recvport | sed -e 's/[^0-9]*//g'`

echo content-type: text/plain

if [ "$sendport" -gt 1024 ] && [ "$recvport" -gt 1024 ]
then
	chime > /dev/null 2>&1
	sendlog=$napdata/naprtpsend.log
	recvlog=$napdata/naprtprecv.log
	touch "$sendlog" "$recvlog"
	chmod a+rw "$sendlog" "$recvlog"
	naprtprecv $sendport >> "$recvlog" 2>&1 &
	naprtpsend "$REMOTE_ADDR" $recvport >> "$sendlog" 2>&1 &
	# for some reason wget on the board is getting stuck
	echo content-length: 9
	echo
	echo CONNECTED
else
	echo content-length: 11
	echo
	echo ERROR ports
fi