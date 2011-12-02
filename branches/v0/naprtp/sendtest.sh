#!/bin/sh
# run recvtest.sh first on the receiving host then run this (both can be same host)
. /etc/profile
if [ "$1" = "" ]
then
	echo error need an ip to send to!
fi
port=29528
echo rtp send to $port
naprtpsend $1 $port
