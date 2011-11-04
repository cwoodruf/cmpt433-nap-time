{pagerlinks howmany=$howmany offset=$offset limit=$limit options=pages action=$action}

{* the following will be captured into a smarty variable *}
{capture name=nav}
<div class="notenav">
{if $offset > 0}
<a href="{$pagerlinks.first}">&lt;&lt; start</a> &nbsp;&nbsp;
<a href="{$pagerlinks.prev}">&lt; prev</a> 
{else}
&lt;&lt; start &nbsp;&nbsp;
&lt; prev 
{/if}
&nbsp;

{assign var=sidelinks value=3}
{foreach from=$pagerlinks.pages key=o item=link}
	{if $o < $offset - $sidelinks * $limit or $o > $offset + $sidelinks * $limit}{php}continue;{/php}{/if}
	{if $o+$limit > $howmany}{assign var=eo value=$howmany}{else}{assign var=eo value=`$o+$limit`}{/if}
	{if $offset >= $o and $offset < $eo}
[{$o+1}-{$eo}]
	{else}
<a href="{$link}">[{$o+1}-{$eo}]</a>
	{/if}
{/foreach}
&nbsp;

{if $offset < $howmany - $limit}
<a href="{$pagerlinks.next}">next &gt;</a> &nbsp;&nbsp;
<a href="{$pagerlinks.last}">last &gt;&gt;</a> 
{else}
next &gt; &nbsp;&nbsp;
last &gt;&gt; 
{/if}
</div>
{/capture}

