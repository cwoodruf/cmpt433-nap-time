{* 
   basic data entry form scaffold - 
   use the $schema array to determine how 
   information is input into the form 
*}
{if !isset($schema) and isset($this->schema)}
	{assign var=schema value=$this->schema}
{/if}
{if !isset($input) and isset($this->input)}
	{assign var=input value=$this->input}
{/if}
{schema schema=$schema}
<form id="formgen" action="{$smarty.server.PHP_SELF}" method="post">
<table cellspacing="0" cellpadding="5" border="0" class="formgen">
<tr class="formgen formbuttons">
<td class="formgen formbuttons">
<input type="reset" value="reset" />
</td>
<td class="formgen formbuttons" align="right">

{* you can use an array instead of action=controller/modifier form *}
{if $this->controller}
{assign var=actvar value="action[]"}
<input type="hidden" name="{$actvar}" value="{$this->controller}" />
{else}
{assign var=actvar value="action"}
{/if}

{include file=tools/hiddenfields.tpl}
{if $action}
<input type="submit" name="{$actvar}" value="{$action}" />
{elseif $this->newaction}
<input type="submit" name="{$actvar}" value="{$this->action}" />
{else}
<input type="submit" name="{$actvar}" value="save" />
{/if}

</td>
</tr>

{foreach from=$schema key=field item=fdata}
{assign var=checked value=""}

{if $fdata.hide}{php}continue;{/php}{/if}

<tr class="formgen" valign="top">
<td class="formgen"><b>{$field}</b></td>
<td class="formgen">
{assign var=value value=$input[$field]}

{if $prefix}
{assign var=fieldname value="$prefix[$field]"}
{elseif $this->prefix}
{assign var=fieldname value="`$this->prefix`[$field]"}
{else}
{assign var=fieldname value=$field}
{/if}

{if $fdata.auto}
 {if isset($value)}
  {$value}
  <input type="hidden" name="{$fieldname}" value="{$value}">
 {else}
  <i>{$fdata.alt}</i>
 {/if}
&nbsp;
{php}continue;{/php}
{/if}

{if $fdata.static}
{$value|htmlentities}
{php}continue;{/php}
{/if}


{if $fdata.plugin}
{$fdata.plugin field=$field data=$fdata}

{elseif $fdata.template}
{include file=$fdata.template field=$field data=$fdata}

{elseif $fdata.type == 'text'}
<textarea name="{$fieldname}" rows="{$fdata.rows}" cols="{$fdata.cols}">{$value}</textarea>

{elseif $fdata.type == 'varchar'}
<input name="{$fieldname}" size="{$fdata.size}" value="{$value}" /> 

{elseif $fdata.type == 'enum' and is_array($fdata.opts)}
<select name="{$fieldname}"><option></option>
{foreach from=$fdata.opts key=i item=option}
	{if $option == $value}{assign var=selected value=selected}{else}{assign var=selected value=''}{/if}
<option value="{$option}" {$selected}>{$option}</option>
{/foreach}
</select>

{elseif $fdata.type == 'radio' and is_array($fdata.opts)}
{foreach from=$fdata.opts key=i item=option}
	{if $option == $value}{assign var=selected value=checked}{else}{assign var=selected value=''}{/if}
<span class="formgen radio">
<input type="radio" name="{$fieldname}" value="{$option}" {$selected} /> {$option} 
</span>
{/foreach}

{elseif $fdata.type == 'checkbox' or $fdata.type == 'boolean'}
	{if $value}{assign var=checked value=checked}{else}{assign var=value value=1}{/if} 
<input type="checkbox" name="{$fieldname}" {$checked} value="{$value}" />

{elseif $fdata.type == 'select' and $fdata.options}
<select name="{$fieldname}"><option></option>
{$fdata.options}
</select>

{elseif $fdata.type == 'password'}
<input type="password" name="{$fieldname}" size="{$fdata.size}" value="{$value}" /> 

{elseif $fdata.type == 'hidden'}
<input type="hidden" name="{$fieldname}" value="{$value}" />

{else}
<input name="{$fieldname}" value="{$value}" /> 
	
{/if}

</td>
</tr>

{/foreach}

</table>
</form>
