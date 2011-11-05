#!/usr/bin/perl
# purpose of this script is to listen for connections on our listening port and
# try and send messages between napmedia client boards
# see http://www.thegeekstuff.com/2010/07/perl-tcp-udp-socket-programming/ for socket original

use POSIX qw/mkfifo/;
use Fcntl qw/:flock/;
use IO::File;
use IO::Select;
use IO::Socket::UNIX;
use IO::Socket::INET;
use PHP::Serialization qw/serialize unserialize/;
use Getopt::Std;
use Data::Dumper;

use strict;
$| = 1; # autoflush

###################### types of requests ######################################
my %reqtype = (
	'peers' => \&peers, # list of currently alive ips
	'index' => \&getindex, # needs a neighbour ip
	'file' => \&getfile, # needs neighbour ip and file id
	'stream' => \&stream, # sends continuous stream of bytes
);

###################### process input ##########################################
my $defport = 3333;
my $defnodedir = '/usr/local/apache2/htdocs/napmedia/nodes';
my $defnodelife = '60';
my $nodelifemin = '1';
my $nodelifemax = '86400';
my %opt;
getopts('p:d:hv',\%opt);
my $usage = <<TXT;
$0 [-p{port}] [-d{node dir}] [-t{ttl}] [-v] [-h]

To run:
$0 [options] >> {logfile} 2>&1 &

Options:
	-p{port} port to listen on for requests ($defport)
	-d{dir} where to find php serialized node data ($defnodedir)
	-t{ttl} time to live in seconds for a node entry ($defnodelife)
	-v verbose output
	-h this help
TXT
print $usage and exit if $opt{h};

my $port = $opt{p} || $defport;
die "port should be a number between 1024 and 65535!\n$usage" 
	unless $port >= 1024 and $port <= 65535;

my $nodedir = $opt{d} || $defnodedir;
die "need a valid node directory!\n$usage" unless -d $nodedir;

my $nodelife = $opt{t} || $defnodelife;
die "node time to live should be between $nodelifemin and $nodelifemax!"
	unless $nodelife >= $nodelifemin and $nodelife <= $nodelifemax;

if ($opt{v}) {
	print "listening on $port\nnode directory $nodedir\nnode ttl $nodelife s\n";
	print "allowed commands:\n\t", join "\n\t", keys %reqtype;
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

while (1) {
	my $clientsock = $listener->accept;

	if (my $pid = fork) {
		print "started worker $pid\n" if $opt{v};
	} else {
		# keep track of all the other nodes that have connected to you 
		exit 1 unless (my $fifo = &save_connection($clientsock));
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
						print "ACKing\n" if $opt{v};
						print $clientsock "ACK\n";
					}
				} elsif ($reader == $fifo) {
					my $fifoconn = $fifo->accept;
					my $fiforaw = <$fifoconn>;
					print "from fifo to client: $fiforaw\n" if $opt{v};
					print $clientsock $fiforaw;
					my $cbuff;
					while (<$clientsock>) {
						print "from client to fifo: $_\n" if $opt{v};
						$cbuff .= $_;
						last if /^END/;
					}
					print $fifoconn $cbuff;
					$fifoconn->flush;
					# close $fifoconn;
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
	my $fifo = "$nodedir/$ip.sock";
	$node->{fifo} = $fifo;
	$node->{ip} = $ip;
	$node->{port} = $socket->peerport;
	$node->{ts} = time;
	if (&nodesave($ip,$node)) {
		unlink $fifo;
		my $local = IO::Socket::UNIX->new(
			Local => $fifo,
			Type => SOCK_STREAM,
			Listen => 10,
		) or warn "can't open local socket: $!" and return;
		return $local;
	}
	return;
}

sub valid_ip {
	my ($ip) = @_;
	$ip =~ /((?:\d{1,3}\.){3}\d{1,3})/;
	return $1;
}

sub nodeload {
	my ($ip) = @_;
	warn "nodeload: $ip wrong form - aborting!" and return 
		unless $ip = &valid_ip($ip);
	my $node;
	local $/ = undef;
	if (open NODE, "$nodedir/$ip.php") {
		flock NODE,LOCK_SH;
		$node = unserialize(<NODE>);
		flock NODE,LOCK_UN;
		close NODE;
	}
	$node;
}

sub nodesave {
	my ($ip, $node) = @_;
	warn "nodesave: $ip wrong form - aborting!" and return 
		unless $ip = &valid_ip($ip);
	warn "nodesave: no node data for $ip!" and return
		unless defined $node;
	print "saving $node->{ip}:$node->{port} $node->{ts} epoch s\n" if $opt{v};
	if (open NODE, "> $nodedir/$ip.php") {
		flock NODE,LOCK_SH;
		print NODE serialize($node);
		flock NODE,LOCK_UN;
		close NODE;
		return 1;
	}
	warn "error opening $nodedir/$ip.php: $!";
	0;
}

sub parse_req {
	my ($socket, $buff) = @_;
	chomp $buff;
	my ($cmd,$data) = ($buff=~/^(\w+)(.*)/);
	$data =~ /^\s*/; 
	$data =~ /\s*$/s;
	return unless defined $reqtype{$cmd};
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
	return &{$reqtype{$req->{cmd}}}($req);
}
######################## callbacks #####################################
# list of currently alive ips
sub peers {
	my ($req) = @_;
	my $sock = $req->{sock};
	foreach my $ipfile (glob "$nodedir/[0-9]*.[0-9]*.[0-9]*.[0-9]*.php") {
		my $ip = valid_ip($ipfile);
		my $node = &nodeload($ip);
		next unless $node;
		# if we haven't heard from someone assume they are no longer with us
		unlink $node->{fifo} and next if time - $node->{ts} > $nodelife;
		print "$ip $node->{desc}\n" if $opt{v};
		print $sock "$ip $node->{desc}\n";
	}
	print $sock "END\n";
	1;
}
# needs a neighbour ip
sub getindex {
	my ($req) = @_;
	my ($theirip) = ($req->{data} =~ m#(\S+)#);
	my $node = nodeload($theirip);
	warn "getindex: no node for $theirip!" and return unless defined $node;
	warn "no peer socket for $theirip!" and return unless -S $node->{fifo};
	my $fifoconn = IO::Socket::UNIX->new(
		Peer => $node->{fifo},
		Type => SOCK_STREAM,
		Timeout => 2,
	) or warn "getindex: can't open socket: $!";
	my $sel = IO::Select->new;
	$sel->add($fifoconn);
	print $fifoconn "index\n";
	$fifoconn->flush;
	my @readers = $sel->can_read;
	foreach my $reader (@readers) {
		next unless $reader == $fifoconn;
		my $reqsock = $req->{sock};
		while (my $data = <$fifoconn>) {
			print $reqsock $data;
		}
	}
	close $fifoconn;
	1;
}

# needs neighbour ip and file id
sub getfile {
	my ($req) = @_;
	1;
}
# sends continuous stream of bytes
# needs a neighbour ip
sub stream {
	my ($req) = @_;
	1;
}
__END__
Monday, September 19, 2005
Unix Domain Sockets in Perl

I know this has been done dozens of times before, but it's been a real headache for me lately. A full example of how to use IO::Socket::UNIX is not really available anywhere on the web. All of the socket info for perl seems to assume you've been doing socket programming in C for years, and theres very limited info on doing Unix Domain Sockets. it all seems to be about inet sockets. which, of course, relates a lot to Unix Sockets, but there are some differences.

okay, here is my working example. Its meant to be run twice, the first time it's the server, the second time it's the client.



#!/usr/bin/perl
use strict; $|++;
use IO::Socket;

my $socketfile = $ENV{HOME} . "/.sockettest";

if ( -S $socketfile ) {
 # client!
 my $client = IO::Socket::UNIX->new(Peer  => $socketfile,
                                    Type      => SOCK_STREAM ) or die $!;
 my $string = "this is some sent garbage.\n";
 print $client $string;
 $client->flush;
 $client->close;
 exit;

} else {
 # server!
 unlink $socketfile;
 my $data;
 my $server = IO::Socket::UNIX->new(Local => $socketfile,
                                    Type      => SOCK_STREAM,
                                    Listen    => 32 ) or die $!;
 $server->autoflush(1);
 while ( my $connection = $server->accept() ) {
   my $data= <$connection>;
   print $data, $/;

   sleep 5;
 }
}
