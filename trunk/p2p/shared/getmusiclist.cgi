#!/bin/sh
. /etc/nap.conf
echo content-type: text/plain
echo
find  -name "*.mp3" -exec echo "http://$SERVER_ADDR...{}" \; | sed -e 's/\.\.\.\.//'
