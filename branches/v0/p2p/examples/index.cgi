#!/bin/sh
# how to do replace on a template
qstr=$QUERY_STRING
html=`cat index.html`
echo content-type: text/html 
#echo content-type: text/plain
echo
for name in `cginames $qstr`
do
	replace=`cgiparser -n$name\= "$qstr"`
	html=`echo $html | sed -e "s#<$name>#$replace#"`
done

echo $html
