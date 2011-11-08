#!/usr/bin/perl
# purpose of this script is to listen for connections on our listening port and
# try and send messages between napmedia client boards
# see http://www.thegeekstuff.com/2010/07/perl-tcp-udp-socket-programming/ for inet socket original
# see http://www.rexroof.com/blog/2005/09/unix-domain-sockets-in-perl.php for unix socket example

use lib qw{/usr/local/apache2/htdocs/napmedia/naptime};

use Nap;
use IO::File;
use IO::Select;
use IO::Socket::UNIX;
use IO::Socket::INET;
use Getopt::Std;
use Data::Dumper;

use strict;
$| = 1; # autoflush

###################### process input ##########################################
our %opt;
getopts('P:p:d:hv',\%opt);
my $usage = <<TXT;
$0 [-P{pwfile}] [-p{port}] [-d{node dir}] [-t{ttl}] [-v] [-h]

To run:
$0 [options] >> {logfile} 2>&1 &

Options:
	-P{pwfile} where to find validation password file ($pwfile)
	-p{port} port to listen on for requests ($napport)
	-d{dir} where to find php serialized node data ($napnodedir)
	-t{ttl} time to live in seconds for a node entry ($napnodelife)
	-v verbose output
	-h this help
TXT
print $usage and exit if $opt{h};

our $pwfile = $opt{P} || $nappwfile;
die "$pwfile not a file!" unless -f $pwfile;

my $port = $opt{p} || $napport;
die "port should be a number between 1024 and 65535!\n$usage" 
	unless $port >= 1024 and $port <= 65535;

$Nap::nodedir = $opt{d} || $napnodedir;
die "need a valid node directory!\n$usage" unless -d $Nap::nodedir;

$Nap::nodelife = $opt{t} || $napnodelife;
die "node time to live should be between $napnodelifemin and $napnodelifemax!"
	unless $napnodelife >= $napnodelifemin and $napnodelife <= $napnodelifemax;

if ($opt{v}) {
	print "listening on $port\nnode directory $Nap::nodedir\nnode ttl $napnodelife s\n";
	print "allowed commands:\n\t", join "\n\t", keys %napreq;
	print "\n";
}


my $listener = IO::Socket::INET->new(
	LocalPort => $port,
	Proto => 'tcp',
	Listen => 10,
	Reuse => 1, 
) or die "cannot create socket: $!";

######################## process requests #####################################
my $buff;
my $buffsize = 8 * 1024;
$SIG{CHLD} = 'IGNORE'; # automatically reap spent worker processes

open PIDFILE, "> $Nap::nodedir/napbridge.pid" or die "can't save pid in $Nap::nodedir/naptime.pid: $!";
print PIDFILE $$;
close PIDFILE;
print "saved pid $$ to $Nap::nodedir/napbridge.pid\n" if $opt{v};

while (1) {
	my $clientsock = $listener->accept;

	if (my $pid = fork) {
		print "started worker $pid\n" if $opt{v};
	} else {
		# keep track of all the other nodes that have connected 
		my $node;
		my $validate = <$clientsock>;
		my $req = &parse_req($clientsock, $validate);
		if (!defined $req or $req->{cmd} ne 'validate') {
			exit 1;
		} else {
			# the validate callback will either returned what went wrong or a $node
			my $response = &do_req($req);
			if ($response =~ /^ERROR/) {
				print "validate request failed: $response!\n",Dumper($req);
				print $clientsock "$response\n";
				exit 2;
			}
			$node = $response;
			print $clientsock "VALID\n";
		}
		my $fifo = $node->{fifoconn};
		my $sel = IO::Select->new();
		$sel->add($clientsock);
		$sel->add($fifo);

		while (my @readers = $sel->can_read) {
			foreach my $reader (@readers) {
				if ($reader == $clientsock) {
					my $raw = <$clientsock>;
					# print "raw $raw\n" if $opt{v};
					if (my $req = &parse_req($clientsock,$raw)) {
						unless (&do_req($req)) {
							print "request failed!\n",Dumper($req) if $opt{v};
							print $clientsock "FAIL\n";
						}
					}
				} elsif ($reader == $fifo) {
					my $fifoconn = $fifo->accept;
					my $fiforaw = <$fifoconn>;
					# print "from fifo to client: $fiforaw\n" if $opt{v};
					print $clientsock $fiforaw;
					my ($response,$cbuff);
					$response = <$clientsock>;
					print "forwarding response\n$response" if $opt{v};
					if ($response =~ /^RESPONSE (\d+)/) {
						my $bytes = $1;
						$response .= <$clientsock>;
						read($clientsock, $cbuff, $bytes);
					}
					print $fifoconn $response,$cbuff;
					$fifoconn->flush;
				}
			}
		}
	}
}	

sub parse_req {
	my ($socket, $buff) = @_;
	chomp $buff;
	my ($cmd,$data) = ($buff=~/^(\w+)(.*)/);
	$data =~ /^\s*/; 
	$data =~ /\s*$/s;
	return unless defined $napreq{$cmd};
	my $req = {
		cmd => $cmd,
		data => $data,
		peerhost => $socket->peerhost,
		peerport => $socket->peerport,
		sock => $socket,
		ts => time,
	};
	return $req;
}

sub do_req {
	my ($req) = @_;
	print "processing $req->{cmd}\n" if $opt{v};
	return &{$napreq{$req->{cmd}}}($req);
}

