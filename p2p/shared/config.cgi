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
key=""
if [ -f "$napdata/password" ] 
then
	password=`cat $napdata/password`
	if [ "$action" = "login" ]
	then
		encodepw=`encodeparam pw`
		if [ "$password" != "" ] && [ "$encodepw" != "$password" ]
		then 
			cat loginform.html
			return
		else
			key=$REMOTE_ADDR
			key=`date`$key 
			key=`echo "$key" | sha1sum | sed -e 's/[^a-f0-9]*//g'`
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
	echo `param name` | sed -e 's/[^a-zA-Z0-9_ \-]//g' > "$napdata/name"
;;
naplistener*)
	naplistenerip=`param naplistenerip | sed -e 's/^00*//' -e 's/[^0-9]//g'`
	if [ $naplistenerip -gt 255 ] || [ $naplistener -lt 1 ]
	then 
		error="ip triplet should be between 1 and 255"
		naplistenerip=""
	fi
	naplistenerport=`param naplistenerport | sed -e 's/^00*//' -e 's/[^0-9]//g'`
	if [ $naplistenerport -lt 1024 ] || [ $naplistenerport -gt 65535 ]
	then
		error="port should be between 1025 and 65535"
		naplistenerport=""
	fi
	ok=""
	if [ "$naplistenerip" != "" ]
	then
		echo "$naplistenerip" > "$napdata/naplistenerip"
		ok="set ip"
	else 
		rm "$napdata/naplistenerip"
		ok="deleted ip"
	fi
	if [ "$naplistenerport" != "" ]
	then
		echo "$naplistenerport" > "$napdata/naplistenerport"
		ok="$ok set port"
	else 
		rm "$napdata/naplistenerport"
		ok="$ok deleted port"
	fi
	if [ "$ok" != "" ] ; then ok="updated naplistener: $ok"; fi
	
;;
changepassword*)
	oldpw=`encodeparam oldpw`
	if [ "$password" = "" ] || [ "$oldpw" = "$password" ]
	then
		newpw=`param newpw`
		newpwconf=`param newpwconf`
		if [ "$newpw" = "$newpwconf" ]
		then
			if [ "$newpw" = "" ]
			then
				ok="deleted password"
				rm "$napdata/password"
			else 
				echo "$newpw" | sha1sum > "$napdata/password"
				ok="changed password"
			fi
		else
			error="new passwords don't match"
		fi
	else
		error="wrong old password $password $oldpw"
	fi
;;
esac
name=`cat $napdata/name`
naplistenerip=`cat $napdata/naplistenerip`
naplistenerport=`cat $napdata/naplistenerport`
hiddenkey="<input type=hidden name=key value=\"$key\">"

# print out the form 
tbwidth=500
cat <<HTML
<html>
<head>
<title>Nap configuration $ip</title>
<link rel=stylesheet type=text/css href=napstyle.css>
</head>
<body>
<h4>Nap player configuration ($SERVER_NAME $SERVER_ADDR - dir: $napdata)</h4>
<a href="about.html">About Nap Player</a>
<div class="error">$error</div>
<div class="ok">$ok</div>
<pre>$response</pre>
<form action=config.cgi method=get>
$hiddenkey
<h4>Change System name</h4>
<table cellpadding=5 cellspacing=0 border=0 width=$tbwidth>
<tr><td>
New system name
</td>
<td align=right>
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
$hiddenkey
<input type=hidden name=action value=naplistener>
<h4>Change <a href=naplistener.html>naplistener</a> parameters</h4>
<table cellpadding=5 cellspacing=0 border=0 width=$tbwidth>
<tr><td>
naplistener multicast IP address
</td><td align=right>
225.0.0.<input name=naplistenerip value="$naplistenerip" size=6>
</td>
</tr>
<tr><td>
naplistener port
</td><td align=right>
<i>(between 1025 and 65535)</i> 
<input name=naplistenerport value="$naplistenerport" size=6>
</td></tr>
<tr>
<td><input type=reset value=resest></td>
<td align=right><input type=submit value="Update naplistener"></td>
</td></tr>
</table>
</form>
<form action=config.cgi method=get>
$hiddenkey
<input type=hidden name=action value="changepassword">
<h4>Change password</h4>
<table cellpadding=5 cellspacing=0 border=0 width=$tbwidth>
<tr><td>
Old password 
</td>
<td align=right>
<input type=password name=oldpw size=30><br><i>(leave blank if you currently have no password)</i>
<p>
</td>
<tr>
<td>
New password 
</td>
<td align=right>
<input type=password name=newpw size=30>
</td>
</tr>
<tr>
<td>
Confirm password 
</td>
<td align=right>
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

