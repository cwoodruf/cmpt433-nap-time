#!/bin/sh
# get the connection parameters and ask the end user for confirmation from the gui
. /etc/nap.conf

ip=$REMOTE_ADDR
sendport=`param sendport`
recvport=`param recvport`

echo content-type: text/plain
echo
echo send $sendport recv $recvport

if [ "$sendport" -gt 1024 ] && [ "$recvport" -gt 1024 ]
then
	chime
	naprtprecv $sendport &
	naprtpsend "$REMOTE_ADDR" $recvport &
	echo CONNECTED
else
	echo ERROR missing ports
fi
