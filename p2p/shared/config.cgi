#!/bin/sh
peerdir=/tmp/peers
ip=`../ip.sh`
action=`param action`
peers=`ls $peerdir | grep -v name | sed -e s#.*/## | sed -e "s#\(.*\)#<a href=\"http://\1/config.cgi\">\1</a><br>#"`

case $action in
peers*)
	response=`napping -t1 -d$peerdir`
;;
rename*)
	echo `param name` > $peerdir/name
;;
showname*)
	echo content-type: text/plain
	echo
	cat $peerdir/name
	return
;;
theirname*)
	theirip=`param ip`
	theirport=`param port`
	url="http://$theirip$theirport/config.cgi?action=showname"
	theirname=`wget -q -O- $url`
	response=`napping -t1 -d$peerdir`
	response="$theirip $theirname $url"
	echo $theirname > $peerdir/$theirip/name
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
<h3>peers</h3>
$peers
</body>
</html>

HTML

