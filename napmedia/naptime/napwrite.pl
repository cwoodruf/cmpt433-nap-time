#!/usr/bin/perl
use IO::Socket::UNIX;
use Data::Dumper;
use File::Path;
use strict;
my $client;
$SIG{INT} = sub { $client->close; exit; };

while (<STDIN>) {
	$client = IO::Socket::UNIX->new(
		Peer => "$ENV{HOME}/napclient.sock",
		Type => SOCK_STREAM,
	) or die $!;

	print $client $_;
	$client->flush;
	my $response = <$client>;
	print "$response";
	if ($response =~ /^RESPONSE (\d+) +(\S.*)/) {
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
		while (<$client>) {
			print $_;
		}
	}
	$client->close;
}

