{aryassign ary=$this->input keys='confirm,what,action,submit'}
<h3>{$confirm|htmlentities}<h3>

<form name="confirm" method="post">
{if $action}
<input type="hidden" name="what" value="{$what|htmlentities}">
<input type="hidden" name="action" value="{$action|htmlentities}">
<input type="submit" value="{$submit|default:ok|htmlentities}">
{/if}
</form>
