#!/usr/bin/perl
# purpose of this script is to listen for connections on our listening port and
# try and send messages between napmedia client boards
# see http://www.thegeekstuff.com/2010/07/perl-tcp-udp-socket-programming/ for inet socket original
# see http://www.rexroof.com/blog/2005/09/unix-domain-sockets-in-perl.php for unix socket example

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
getopts('H:p:d:hv',\%opt);
my $usage = <<TXT;
$0 [-p{port}] [-d{node dir}] [-t{ttl}] [-v] [-h]

To run:
$0 [options] >> {logfile} 2>&1 &

Options:
	-p{port} port to listen on for requests ($napport)
	-d{dir} where to find php serialized node data ($napnodedir)
	-t{ttl} time to live in seconds for a node entry ($napnodelife)
	-v verbose output
	-h this help
TXT
print $usage and exit if $opt{h};

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
		# keep track of all the other nodes that have connected to you 
		exit 1 unless (my $node = &save_connection($clientsock));
		my $fifo = $node->{fifoconn};
		my $sel = IO::Select->new();
		$sel->add($clientsock);
		$sel->add($fifo);

		while (my @readers = $sel->can_read) {
			foreach my $reader (@readers) {
				if ($reader == $clientsock) {
					my $raw = <$clientsock>;
					print "raw $raw\n" if $opt{v};
					if (my $req = &parse_req($clientsock,$raw)) {
						unless (&do_req($req)) {
							print "request failed!\n",Dumper($req) if $opt{v};
							print $clientsock "FAIL\n";
						}
					} else {
						print "ACKing $node->{ip}\n" if $opt{v};
						print $clientsock "ACK\n";
					}
				} elsif ($reader == $fifo) {
					my $fifoconn = $fifo->accept;
					my $fiforaw = <$fifoconn>;
					print "from fifo to client: $fiforaw\n" if $opt{v};
					print $clientsock $fiforaw;
                                        my $response;
                                        $response = <$clientsock>;
                                        if ($response =~ /^RESPONSE (\d+)/) {
                                                my $bytes = $1;
                                                <$clientsock>;
                                                $clientsock->recv($clientsock, $response, $bytes);
                                        }
                                        print "forwarding response\n$response";
                                        print $fifoconn $response;
                                        $fifoconn->flush;
				}
			}
		}
	}
}	

# see http://www.rexroof.com/blog/2005/09/unix-domain-sockets-in-perl.php
# for example of how to use unix domain sockets
sub save_connection {
	my ($socket) = @_;
	my $ip = $socket->peerhost;
	my $node = &nodeload($ip);
	my $fifo = "$Nap::nodedir/$ip.sock";
	$node->{fifo} = $fifo;
	$node->{ip} = $ip;
	$node->{port} = $socket->peerport;
	$node->{ts} = time;
	# this unix socket is meant to live for the life of the process
	if (&nodesave($ip,$node)) {
		unlink $fifo;
		my $local = IO::Socket::UNIX->new(
			Local => $fifo,
			Type => SOCK_STREAM,
			Listen => 10,
		) or warn "can't open local socket: $!" and return;
		# pointless to save this to file - so its set here
		$node->{fifoconn} = $local;
		return $node;
	}
	return;
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
