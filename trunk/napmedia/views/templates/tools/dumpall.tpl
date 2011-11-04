{if !isset($dumptpl)}
	{assign var=dumptpl value=tools/dump.tpl}
{/if}

<table cellpadding="5" cellspacing="0" border="0" class="{$class}">

{foreach from=$list key=num item=data}
<tr><td>
	{include file=$dumptpl}
</td></tr>
{foreachelse}
<tr><td>No records</td></tr>

{/foreach}

</table>

