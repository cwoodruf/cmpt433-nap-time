#!/usr/bin/perl
use IO::Socket::UNIX;
use Data::Dumper;
use File::Path;
use strict;
my $client;
$SIG{INT} = sub { $client->close; exit; };

my $sockfile = shift;
die "need a connected socket file!" unless -S $sockfile;

while (<STDIN>) {
	$client = IO::Socket::UNIX->new(
		Peer => $sockfile,
		Type => SOCK_STREAM,
	) or die "$sockfile: $!";

	print $client $_;
	$client->flush;
	my $response = <$client>;
	print "$response";
	if ($response =~ /^RESPONSE (\d+) *(\S.*)/) {
		my $bytes = $1;
		my $fname = $2;
		my $dir = ($fname=~m#(.*)/#)[0];
		mkpath $dir unless $dir eq '' or -d $dir;
		open FILE, "> $fname" or die "can't open file: $!";
		<$client>;
		my $cbuff;
		while (read($client,$cbuff,$bytes)) {
			print FILE $cbuff;
		}
		close FILE;
		print Dumper([stat($fname)]);
	} else {
		print "just dumping out the rest of the response\n";
		while (<$client>) {
			print $_;
		}
	}
	$client->close;
}

