#!/bin/sh
echo content-type: text/plain
echo

source /etc/nap.conf
memo=`param memo`
name=`param name | sed 's/[^a-zA-Z0-9][^a-zA-Z0-9]*/-/g'`
host=`param host | sed 's/[^a-zA-Z0-9][^a-zA-Z0-9]*/-/g'`
path="$napdata/memos/+$name""_$host""_$memo"

touch "$napdata/memos/-"
wget -O "$path" "http://$REMOTE_ADDR/memos/$memo" && echo OK "$path"
chmod a+r "$path"

