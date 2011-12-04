#!/bin/sh
# based on naprtpsend {ip} {port} [x86]
# must be careful to avoid overruns 
echo content-type: text/plain
echo
. /etc/profile
. /etc/nap.conf
PATH=".:$PATH"
sendport=`param sendport`
echo START `date` >> "$napdata/naprtpsend.log"
strace arecord -f cd -c 1 -r 4000 -t raw | rtpsend-arm - $REMOTE_ADDR $sendport >> "$napdata/naprtpsend.log" 2>&1 

