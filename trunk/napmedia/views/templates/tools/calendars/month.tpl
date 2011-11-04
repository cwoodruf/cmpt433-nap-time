{assign var=dom value=0}
{if $showevents == ''}
	{assign var=showevents value=tools/calendars/showevents.tpl}
{/if}

<div class="calendar-title">{$month} {$year}</div>

<div class="calendar-form">
<form name="pickdate">
<input type="hidden" name="action" value="{$this->action}">
<input type="hidden" name="startdate">
Highlight date: {html_select_date start_year=2009 end_year=2011}
<input type="submit" value="Go">
&nbsp;&nbsp;
<a href="?action={$this->action}&startdate={$prevdate}">&lt; Previous Month</a> &nbsp;&nbsp;
<a href="?action={$this->action}&startdate={$nextdate}">Next Month &gt;</a>
</form>
</div>

<div class="calendar">
{foreach from=$weeks key=i item=week}
<div class="calendar month eow"></div>

{foreach from=$days key=dow item=day}

{if $firstdow == $dow}
	{if $dom == 0}
		{assign var=dom value=1}
	{/if}
{/if}

{if $dom > 0 and $dom <= $eom}
	{capture assign=date}{$year}-{$mon}-{$dom|string_format:"%02d"}{/capture}
	{if $dom == $startday}
	<div class="calendar month selected dom">
	{elseif $dow == 7 or $dow == 6}
	<div class="calendar month weekend dom">
	{else}
	<div class="calendar month dom">
	{/if}
		
		{$day|capitalize}<br>{$date} &nbsp;
		{include file=$showevents date=$date events=$events[$date]}
	</div>
	{assign var=dom value=`$dom+1`}

{else}
	{* hack to put the correct date in days not in the month in question *}
	<div class="calendar month">
		{$day}<br>
	{if $dom == 0}
		{php}
		if (!$pre) $pre = $this->_tpl_vars['firstdow'] + 1;
		else $pre--;
		print date('Y-m-d',strtotime("{$this->_tpl_vars['startdate']} - $pre day"));
		{/php}
	{else}
		{php}
		if (!$post) $post = 1; else $post++;
		print date('Y-m-d',strtotime("{$this->_tpl_vars['enddate']} + $post day"));
		{/php}
	{/if}
	</div>

{/if}

{/foreach}

{/foreach}
</div>
