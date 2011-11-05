<a href="?action=node">Node setup</a>  -  
<a href="?action=upload">Upload mp3 files</a> 
<h4>Napbridge</h4>
<i><b>napbridge.pl</b> is a daemon process that can forward commands from one node to another.<br>
Use address drdata.co.cc:29533 via tcp to connect.
</i>
<p>
<i>commands:
<ul>
<li>peers - get a list of nodes we know about
<li>index [ip] - get the list of content from a node at ip [ip]
<li>getfile [ip] [fileid] - get bytes from file identified by [fileid] hash
<li>stream [ip] - send a stream of bytes to ip [ip]
</ul>
</i>
<p>
<b>{$bridgestatus}</b> 
<p>
<a href="?action=home/restartbridge">Restart napbridge.pl</a> - 
<a href="napbridge.txt">View napbridge.pl log</a>

<h4>Nodes</h4>
<pre>
{$nodes|default:none}
</pre>
<br>
<h4>Local files</h4>
<pre>
{$files|default:none}
</pre>
