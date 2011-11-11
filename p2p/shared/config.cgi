#!/bin/sh
peerdir=/tmp/peers
ip=`../ip.sh`
action=`cgiparser -naction\= $QUERY_STRING`
case $action in
peers*)
	response=`napping -t1 -d$peerdir`
;;
rename*)
	echo `cgiparser -nname\= $QUERY_STRING` > $peerdir/name
;;
getname*)
	echo content-type: text/plain
	echo
	cat $peerdir/name
	return
;;
esac
name=`cat $peerdir/name`

cat <<HTML
content-type: text/html

<html>
<head>
<title>Nap configuration $ip</title>
</head>
<body>
<h3>Nap player configuration</h3>
IP address: $ip
<br>
System name: $name
<br>
<b>$action</b>
<pre>$response</pre>
<form action=config.cgi method=get>
<input name=name value="$name">
<input type=submit name=action value="rename">
</form>
</body>
</html>

HTML
