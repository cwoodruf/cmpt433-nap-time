#!/bin/sh
echo content-type: text/html
echo

source /etc/nap.conf

if [ ! -d "$napdata/keys" ]
then
	mkdir -p "$napdata/keys"
fi
action=`param action`

# if we have a password then do a login challenge and/or check for a valid key
if [ -f "$napdata/password" ] 
then
	source "$napdata/password"
	if [ "$action" = "login" ]
	then
		encodepw=`param pw | sha1sum`
		if [ "$loginpw" != "$password" ]
		then 
			cat loginform.html
			return
		else
			key=$REMOTE_ADDR
			key=`date`$key 
			key=`echo "$key" | sha1sum | sed -e "s/[^a-f0-9]*//"`
			echo "$REMOTE_ADDR" > "$napdata/keys/$key"
		fi
	else 
		key=`param key`	
		if [ -f "$napdata/keys/$key" ]
		then
			touch "$napdata/keys/$key"
		else
			cat loginform.html
			return
		fi
	fi
fi

# now do an action
case $action in
peers*)
	ok=`napping -t1 -d$napdata`
;;
rename*)
	echo `param name` > $napdata/name
;;
changepassword*)
	oldpw=`param oldpw`
	if [ "$oldpw" = `echo "$newpw" | sha1sum` ]
	then
		newpw=`param newpw`
		newpwconf=`param newpwconf`
		if [ "$newpw" = "$newpwconf" ]
		then
			echo "$newpw" | sha1sum | sed -e 's/^/export password=/' > "$napdata/password"
			ok="changed password"
		else
			error="new passwords don't match"
		fi
	else
		error="wrong old password"
	fi
;;
esac
name=`cat $napdata/name`

tbwidth=500
cat <<HTML
<html>
<head>
<title>Nap configuration $ip</title>
<link rel=stylesheet type=text/css href=napstyle.css>
</head>
<body>
<h4>Nap player configuration ($SERVER_NAME $SERVER_ADDR)</h4>
<a href="about.html">About Nap Player</a>
<br>
<b>$error</b><b>$ok</b>
<pre>$response</pre>
<form action=config.cgi method=get>
<h4>Change System name</h4>
<table cellpadding=5 cellspacing=0 border=0 width=$tbwidth>
<tr><td>
New system name
</td>
<td>
<input name=name value="$name" size=30>
</td>
</tr>
<tr>
<td align=right colspan=2>
<input type=submit name=action value="rename">
</td>
</tr>
</table>
</form>
<form action=config.cgi method=get>
<input type=hidden name=action value="changepassword">
<h4>Change password</h4>
<table cellpadding=5 cellspacing=0 border=0 width=$tbwidth>
<tr><td>
Old password 
</td>
<td>
<input type=password name=oldpw size=30><br><i>(leave blank if you currently have no password)</i>
<p>
</td>
<tr>
<td>
New password 
</td>
<td>
<input type=password name=newpw size=30>
</td>
</tr>
<tr>
<td>
Confirm password 
</td>
<td>
<input type=password name=newpwconf size=30>
</td>
<tr>
<td><input type=reset value=resest></td>
<td align=right><input type=submit value="Change password"></td>
</tr>
</table>
</form>
</body>
</html>

HTML

