#!/bin/sh
# get the connection parameters and ask the end user for confirmation from the gui
. /etc/nap.conf

ip=$REMOTE_ADDR
sendport=`param sendport | sed -e 's/[^0-9]*//g'`
recvport=`param recvport | sed -e 's/[^0-9]*//g'`

echo content-type: text/plain

if [ "$sendport" -gt 1024 ] && [ "$recvport" -gt 1024 ]
then
	sendlog=$napdata/naprtpsend.log
	recvlog=$napdata/naprtprecv.log
	touch "$sendlog" "$recvlog"
	chmod a+rw "$sendlog" "$recvlog"
	echo START `date` >> $sendlog
	echo START `date` >> $recvlog

	naprtpstop 2>/dev/null
	naprtprecv $sendport >> "$recvlog" 2>&1 &

	# for some reason wget on the board is getting stuck
	echo content-length: 9
	echo
	echo CONNECTED
	chime > /dev/null 2>&1
else
	echo content-length: 11
	echo
	echo ERROR ports
fi
