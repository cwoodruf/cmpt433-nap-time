#!/usr/bin/perl
# purpose of this script is to listen for connections on our listening port and
# try and send messages between napmedia client boards
# see http://www.thegeekstuff.com/2010/07/perl-tcp-udp-socket-programming/ for socket original

use Fcntl qw/:flock/;
use IO::Socket::INET;
use PHP::Serialization qw/serialize unserialize/;
use Getopt::Std;

use strict;
$| = 1; # autoflush

###################### types of requests ######################################
my %reqtype = (
	'NEIGHBOURS' => \&neighbours, # list of currently alive ips
	'GETINDEX' => \&getindex, # needs a neighbour ip
	'GETFILE' => \&getfile, # needs neighbour ip and file id
	'STREAM' => \&stream, # sends continuous stream of bytes
);

###################### process input ##########################################
my %nodes;
my $defport = 3333;
my $defnodedir = '/usr/local/apache2/napmedia/nodes';
my $defnodelife = '60';
my $nodelifemin = '1';
my $nodelifemax = '86400';
my %opt;
getopts('p:d:h',\%opt);
my $usage = <<TXT;
$0 [-p{port}] [-d{node dir}] [-t{ttl}] [-h]
	-p{port} port to listen on for requests ($defport)
	-d{dir} where to find php serialized node data ($defnodedir)
	-t{ttl} time to live in seconds for a node entry ($defnodelife)
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

my $listener = IO::Socket::INET->new(LocalPort=>$port,Proto=>'udp')
	or die "cannot create socket: $!";

######################## process requests #####################################
my $buff;
my $buffsize = 8 * 1024;

while (1) {
	$listener->recv($buff,$buffsize);

	# keep track of all the other nodes that have connected to you 
	&save_connection($listener);

	# now see if we have anything to do
	if (my $req = &parse_req($listener, $buff)) {
		if (my $pid = fork) {
			waitpid($pid,0);
		} else {
			print $listener "FAIL\n" unless &do_req($req);
		}
	} else {
		# keep socket alive - nodes will continually send keep alive packets
		print $listener "ACK\n";
	}
}	

sub save_connection {
	my ($socket) = @_;
	my $ip = $socket->peeraddr;
	my $node = &nodeload($ip);
	$node->{ip} = $ip;
	$node->{port} = $socket->peerport;
	$node->{ts} = time;
	&nodesave($ip);
	$nodes{$ip} = $node;
}

sub valid_ip {
	my ($ip) = @_;
	return $ip =~ /^(?:\d{1,3}\.){3}\d{1,3}$/;
}

sub nodeload {
	my ($ip) = @_;
	warn "nodeload: $ip wrong form - aborting!" and return 
		unless &valid_ip($ip);
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
		unless &valid_ip($ip);
	warn "nodesave: no node data for $ip!" and return
		unless defined $node;
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
	my ($cmd,$data) = ($buff=~/^([A-Z]+) (.*)/);
	return unless defined $reqtype{$cmd};
	my $req = {
		cmd => $cmd,
		data => $data,
		peeraddr => $socket->peeraddr,
		peerport => $socket->peerport,
		sock => $socket,
		ts => time,
	};
}

sub do_req {
	my ($req) = @_;
	return &{$reqtype{$req->{cmd}}}($req);
}
######################## callbacks #####################################
# list of currently alive ips
sub neighbours {
	my ($req) = @_;
	my $sock = $req->{sock};
	foreach my $ip (keys %nodes) {
		# if we haven't heard from someone remove them
		if (time - $nodes{$ip}->{ts} > $nodelife) {
			delete $nodes{$ip};
		}
		print $sock "$ip $nodes{$ip}{desc}\n";
	}
}
# needs a neighbour ip
sub getindex {
	my ($req) = @_;
	my ($ip) = ($req->data=~m#(\S+)#);
	return unless defined $nodes{$ip};
	my $nodesock = IO::Socket::INET->new(
		PeerAddr => "$ip:$nodes{$ip}{port}",
		Proto => 'udp',
	) or warn "getindex socket error: $!" and return;
	print $nodesock "GETINDEX $req->{ip}\n";
	my $reqsock = $req->{sock};
	while (my $data = <$nodesock>) {
		print $reqsock $data;
	}
	close $nodesock;
}
# needs neighbour ip and file id
sub getfile {
	my ($req) = @_;
}
# sends continuous stream of bytes
# needs a neighbour ip
sub stream {
	my ($req) = @_;
}
