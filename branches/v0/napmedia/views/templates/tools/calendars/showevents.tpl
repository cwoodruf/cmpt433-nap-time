{foreach from=$events key=id item=event}
<nobr>
<a href="?action={$event._action_}&{$event._id_}={$id}">{$event.email} {$id}</a>
</nobr>
<br>
{/foreach}
