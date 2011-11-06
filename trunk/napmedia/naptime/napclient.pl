#!/usr/bin/perl
# purpose of this script is to implement a simple nap client
# probably this would be a shell script and a c program on the EM2440 board 
# idea is to send commands to this client via a unix socket and wait for a response
use Nap qw/$naphost %napreq $napport $napnodelife/;
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

while (1) {
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

	# try and open a connection with the server
	unless (defined $remotesock and $remotesock->connected) {
		$sel->remove($remotesock) if defined $remotesock;
		$remotesock = IO::Socket::INET->new(
			PeerHost => "$naphost:$napport",
			Proto => 'tcp',
		) or warn "can't connect to $naphost:$napport: $!" and next;
		print "established connection to $naphost:$napport\n";
		$remotesock->autoflush(1);
		$sel->add($remotesock);
	}

	print "waiting for something ...\n";
	my $localconn;
	while (my @readers = $sel->can_read) {
		print "got something\n";
		foreach my $reader (@readers) {
			if ($reader == $localsock) {
				print "from local socket\n";
				$localconn = $localsock->accept;
				my $data = <$localconn>;
				print $remotesock $data;
				$remotesock->flush;
				print $data;

			} elsif ($reader == $remotesock) {
				print "from remote socket\n";
				if (defined $localconn and $localconn->connected) {
					my $response;
					$response = <$remotesock>;
					if ($response =~ /^RESPONSE (\d+)/) {
						my $bytes = $1;
						<$remotesock>;
						$remotesock->recv($remotesock, $response, $bytes);
					}
					print "forwarding response\n$response";
					print $localconn $response; 
					$localconn->flush;
					$localconn->close;
				} else {
					# in this case we've been asked to do something by a remote peer
					my $request = <$remotesock>;
					print "request: $request";
					my ($cmd, $cmddata) = ($request =~ /(\S*)(.*)/);
					if (defined $napresponse{$cmd}) {
						&{$napresponse{$cmd}}($remotesock, $cmddata);
						$remotesock->flush;
					}
				}
			}
			sleep 1;
		}
	}
	sleep 5;
}

sub cleanup {
	$remotesock->close() if defined $remotesock and $remotesock->connected;
	$localsock->close() if defined $localsock and $localsock->connected;
	unlink $sockfile;
	exit;
}
