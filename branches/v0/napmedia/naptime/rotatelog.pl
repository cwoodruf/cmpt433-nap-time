#!/usr/bin/perl
# rotate the napmedia logfile if it gets too big
$naplog = shift || '/usr/local/apache2/htdocs/napmedia/napbridge.txt';
$maxlog = shift || 1024*100;
$stat = stat($naplog);
exit if $stat[7] < $maxlog;
rename $naplog, "$naplog~";
system "tail > '$naplog' '$naplog~'";
unlink "$naplog~";
