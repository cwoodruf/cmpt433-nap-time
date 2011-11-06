package Nap;
# defines some shared data between server and client bridge programs

use Exporter;
use IO::Socket::UNIX;
use Fcntl qw/:flock/;
use PHP::Serialization qw/serialize unserialize/;
use strict;
our @ISA = qw/Exporter/;
our @EXPORT = qw/%napreq $naphost $napport $napnodedir $napnodelife $napnodelifemin $napnodelifemax 
		valid_ip nodeload nodesave/;

###################### types of requests ######################################
our %napreq = (
	'peers' => \&peers, # list of currently alive ips
	'index' => \&getindex, # needs a neighbour ip
	'getfile' => \&getfile, # needs neighbour ip and file id
	'stream' => \&stream, # sends continuous stream of bytes
);

###################### program defaults #######################################
our $naphost = 'drdata.co.cc';
our $napport = 29533;
our $nodedir = our $napnodedir = '/usr/local/apache2/htdocs/napmedia/nodes';
our $nodelife = our $napnodelife = 60;
our $napnodelifemin = 1;
our $napnodelifemax = 86400;

######################## callbacks #####################################
# list of currently alive ips
sub peers {
	my ($req) = @_;
	my $peers;
	my $sock = $req->{sock};
	foreach my $ipfile (glob "$nodedir/[0-9]*.[0-9]*.[0-9]*.[0-9]*.php") {
		my $ip = valid_ip($ipfile);
		my $node = &nodeload($ip);
		next unless $node;
		# if we haven't heard from someone assume they are no longer with us
		print "found $node->{ip}:$node->{port} $node->{ts} ".time."\n" if $main::opt{v};
		unlink $node->{fifo} and next if time - $node->{ts} > $nodelife;
		$peers .= "$ip $node->{desc}\n"; 
	}
	my $header = "RESPONSE ".length($peers)."\n\n";
	print $header,$peers if $main::opt{v};
	print $sock $header,$peers;
	1;
}
# needs a neighbour ip
sub getindex {
	my ($req) = @_;
	my ($theirip) = ($req->{data} =~ m#(\S+)#);
	my $node = nodeload($theirip);
	warn "getindex: no node for $theirip!" and return unless defined $node;
	warn "no peer socket for $theirip!" and return unless -S $node->{fifo};
	return &forward_req('index',$node,$req);
}
# needs neighbour ip and file id
sub getfile {
	my ($req) = @_;
	my ($theirip, $file) = ($req->{data} =~ m#(\S+) (.*)#);
	my $node = nodeload($theirip);
	warn "getindex: no node for $theirip!" and return unless defined $node;
	warn "no peer socket for $theirip!" and return unless -S $node->{fifo};
	return &forward_req("getfile $file",$node,$req);
}

###################### utility functions ######################################
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
	if (open NODE, "$napnodedir/$ip.php") {
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
	print "saving $node->{ip}:$node->{port} $node->{ts} epoch s\n" if $main::opt{v};
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

# send a request to another node and return the response to the requesting node
sub forward_req {
	my ($command, $node, $req) = @_;
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

1;
