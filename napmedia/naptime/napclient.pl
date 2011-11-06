#!/usr/bin/perl
# purpose of this script is to implement a simple nap client
# probably this would be a shell script and a c program on the EM2440 board 
# idea is to send commands to this client via a unix socket and wait for a response

use lib qw{/usr/local/apache2/htdocs/napmedia/naptime};

use threads ('yield', 'stack_size' => 64*4096, 'exit' => 'threads_only', 'stringify');
use Nap qw/$naphost %napreq $napport/;
use IO::Socket::INET;
use IO::Socket::UNIX;
use IO::Select;
use strict;

my $sockfile = "napclient.sock";

my $sel = IO::Select->new;
my $remotesock;
my $localsock;
my %napresponse = (
	'index' => \&index_response,
	'getfile' => \&getfile_response,
);
sub index_response {
	my ($sock, $data) = @_;
	return &getfile_response($sock, "index.txt");
}
sub getfile_response {
	my ($sock, $data) = @_;
	# remove any .././s at start of file
	my $file = ($data =~ m#^[\s\./]*(.*)#)[0];
	print $sock "FINDFAIL $file\n" and return unless -f $file;
	open FILE, $file or print "OPENFAIL $file\n" and return;
	my $fsize = (stat($file))[7];
	print $sock "RESPONSE $fsize $file\n\n";
	while (<FILE>) {
		print $sock $_;
	}
	close FILE;
	return;
}

$SIG{INT} = \&cleanup;

sub get_password {
	open PW, "password" or die "can't find validation password!";
	my $password = <PW>;
	close PW;
	chomp $password;
	return $password;
}

sub validate {
	my ($remotesock) = @_;
	my $password = &get_password;
	print $remotesock "validate $password\n";
	$remotesock->flush;
	my $valid = <$remotesock>;
	chomp $valid;
	unless ($valid eq 'VALID') {
		warn "could not validate: $valid";
		return 0;
	}
	print "validated ok!\n";
	return 1;
}

sub remoteconnect {
	$sel->remove($remotesock) if defined $remotesock;
	$remotesock = IO::Socket::INET->new(
		PeerHost => "$naphost:$napport",
		Proto => 'tcp',
	) or warn "can't connect to $naphost:$napport: $!" and return;
	print "established connection to $naphost:$napport\n";
	$remotesock->autoflush(1);
	$remotesock->close and return unless &validate($remotesock);
	$sel->add($remotesock);
	1;
}

while (!&remoteconnect) { sleep 5; }

threads->create(
	sub { 
		sleep $Nap::nodelife/2; 
		while ($remotesock->connected) { 
			my @writers = $sel->can_write;
			foreach my $writer (@writers) {
				&validate($remotesock) if $writer == $remotesock; 
			}
			sleep $Nap::nodelife/2; 
		} 
	}
)->detach;

while (1) {
	# try and open a connection with the server
	unless (defined $remotesock and $remotesock->connected) {
		&remotesock or sleep 5 and next;	
	}

	# our local communication channel
	unless (defined $localsock and $localsock->connected) {
		$sel->remove($localsock) if defined $localsock;
		warn "$sockfile still alive - unlinking (is there another client running?)" 
			and unlink $sockfile if -S $sockfile;
		$localsock = IO::Socket::UNIX->new(
			Type => SOCK_STREAM,
			Local => $sockfile,
			Listen => 32,
		) or die "socket error: $!"; # actually need local communications to work at all
		print "server listening on $sockfile\n";
		$localsock->autoflush(1);
		$sel->add($localsock);
	}

	print "waiting for something ...\n";
	my $localconn;
	while (my @readers = $sel->can_read) {
		print "got something\n";
		foreach my $reader (@readers) {
			if ($reader == $localsock) {
				# a local process is asking us to do something
				print "from local socket\n";
				$localconn = $localsock->accept;
				my $data = <$localconn>;
				print $remotesock $data;
				$remotesock->flush;
				print $data;

			} elsif ($reader == $remotesock) {
				print "from remote socket\n";
				if (defined $localconn and $localconn->connected) {
					# finish the request from the local process initiated above
					my $response;
					$response = <$remotesock>;
					print "forwarding response\n$response";
					my $cbuff;
					if ($response =~ /^RESPONSE (\d+)/) {
						my $bytes = $1;
						$response .= <$remotesock>;
						read($remotesock, $cbuff, $bytes);
					}
					print $localconn $response,$cbuff; 
					$localconn->flush;
					$localconn->close;
				} else {
					# do something for a remote peer
					my $request = <$remotesock>;
					print "request: $request";
					my ($cmd, $cmddata) = ($request =~ /(\S*)(.*)/);
					if (defined $napresponse{$cmd}) {
						&{$napresponse{$cmd}}($remotesock, $cmddata);
						$remotesock->flush;
					}
				}
			}
		}
	}
	sleep 5;
}

sub cleanup {
	$remotesock->close if defined $remotesock;
	$localsock->close if defined $localsock;
	unlink $sockfile;
	exit;
}
