#!/usr/bin/php
<?php
$path = "/home/group8/public_html/frame";

// Either
ini_set("include_path",ini_get("include_path").":$path");

// OR
chdir($path);

// before this line:
require_once("lib/init.php");

// then do something (dump everything in the user table)
var_export(Run::me("User","getall"));


