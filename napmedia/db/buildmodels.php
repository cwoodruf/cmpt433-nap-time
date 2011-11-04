#!/usr/bin/php
<?php
$lib = dirname(dirname(__FILE__));
print "run both mysql2schema.pl and makeclasses.php to build out classes for a database\n";

$db = $argv[1];
if (empty($db)) die("need name of database!\n");

if (isset($argv[2])) $thistable = $argv[2];
# this arg can be empty

if (isset($argv[3])) $modeldir = $argv[3];
if (empty($modeldir)) $modeldir = "models/base";

print "usage {$_SERVER['PHP_SELF']} {database '$db'} [table '$thistable'] [model directory: '$modeldir']\n";

if (!file_exists($modeldir)) mkdir($modeldir,0777,true);
$schemafile = "$modeldir/$db-schema.php";
$mysqlfile = "$modeldir/$db.mysql";

print "These user credentials will be saved for db access.\n";
print "mysql user: ";
if (($stdin = fopen('php://stdin','r')) !== false) {
	$myuser = fgets($stdin,255);
	$myuser = preg_replace('#[\n\r]#','',trim($myuser));
	print "mysql password: ";
	$mypw = fgets($stdin,255);
	$mypw = preg_replace('#[\n\r]#','',trim($mypw));
	shell_exec(
		"mysqldump -u'$myuser' -p'$mypw' --opt --no-data $db ".
		"| /usr/bin/tee $mysqlfile ".
		"| perl $lib/db/mysql2schema.pl > $schemafile"
	);
	print "wrote table info to $schemafile\n";
	# by default makeclasses.php won't overwrite an existing directory
	$force = true;
	require("$lib/db/makeclasses.php");
} else {
	die("can't get standard input");
}

