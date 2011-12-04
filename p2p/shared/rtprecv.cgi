#!/bin/sh
# based on naprtprecv {port} [x86]
echo content-type: text/plain
echo
. /etc/profile
. /etc/nap.conf
PATH=".:$PATH"
recvport=`param recvport`
rtprecv-$ext - $recvport | aplay -f cd -c 1 -r 4000 -t raw >> "$napdata/naprtprecv.log" 2>&1 

