#!/usr/bin/perl
use IO::Socket::UNIX;
use strict;
my $client;
$SIG{INT} = sub { $client->close; exit; };

while (<STDIN>) {
	$client = IO::Socket::UNIX->new(
		Peer => 'napclient.sock',
		Type => SOCK_STREAM,
	) or die $!;

	print $client $_;
	$client->flush;
	while (<$client>) {
		print $_;
	}
	$client->close;
}

