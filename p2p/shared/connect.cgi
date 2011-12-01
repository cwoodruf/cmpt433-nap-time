#!/bin/sh
# get the connection parameters and ask the end user for confirmation from the gui
ip=$REMOTE_ADDR
name=`param name`
host=`param host`
sendport=`param sendport`
recvport=`param recvport`

echo content-type: text/plain
echo

res=`intercom-confirm-arm "$ip" "$host" "$name"`
if [ "$res" = "OK" ]
then
	naprtprecv $sendport &
	naprtpsend $recvport &
	echo CONNECTED
else 
	echo refused connection
fi

