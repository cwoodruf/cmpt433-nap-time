#!/bin/sh
. /etc/nap.conf
echo content-type: text/plain
echo
find  -follow -type f -name "*.mp3" -exec echo "http://$SERVER_ADDR...{}" \; | sed -e 's/\.\.\.\.//'
