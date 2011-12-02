{assign var=tbwidth value=650}
<form action="index.php" method="post">
<table cellpadding=3 cellspacing=0 border=0 width={$tbwidth}>
<tr valign=top><td style="border: 1px black solid; padding: 5px;">
<input type="hidden" name="action" value="node/add">
IP: <input size="16" name="ip">
Description: <input size="20" name="desc">
</td>
<td align=right>
<input type="submit" name="submit" value="Save node">
</td>
</tr>
</table>
</form>
{if $ips}
<form action="index.php" method="post" onsubmit="return confirm('Really delete these node entries?');" >
<input type="hidden" name="action" value="node/del">
<table cellpadding=3 cellspacing=0 border=0 width={$tbwidth}>
<tr valign=top><td style="border: 1px black solid; padding: 5px;">
{foreach from=$ips key=ip item=i}
<input type=checkbox name="delnode[{$ip}]" value=1>  
{$ip} (<a href="?action=node/content&ip={$ip}">view/edit</a>) {$i.desc}
<br>
{/foreach}
</td>
<td align=right>
<input type="submit" name="submit" value="Delete nodes">
</td>
</tr>
</table>
</form>
<a href="?action=node/status">Ping all</a>
{else}
<h4>No nodes</h4>
{/if}

{if isset($responses)}
<h3>Status</h3>
{foreach from=$ips key=ip item=i}
<b>{$ip}</b> {$responses.$ip} ({$i.desc})<br>
{/foreach}
{/if}

{if isset($nodecontent)}
<h3>Content available on {$ip}</h3>
{foreach from=$nodecontent key=hash item=n}
{$n.path}<br>
{/foreach}
{/if}
