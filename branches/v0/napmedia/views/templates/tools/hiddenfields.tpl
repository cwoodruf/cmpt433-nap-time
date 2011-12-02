{if is_array($hidden)}
{foreach from=$hidden key=field item=value}
<input type=hidden name="{$field}" value="{$value}">
{/foreach}

{elseif is_array($this->hidden)}
{foreach from=$this->hidden key=field item=value}
<input type=hidden name="{$field}" value="{$value}">
{/foreach}

{/if}

