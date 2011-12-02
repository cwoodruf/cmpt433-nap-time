<form class="formgen" method="get">
<input type="hidden" name="action" value="{$this->action|regex_replace:'#(/save)+$#':''}/save">
<h3>Change password for {$smarty.session.login.login|@htmlentities}</h3>
<table cellpadding="3" cellspacing="0" border="0" class="formgen">
<tr><td>Old Password:</td><td><input size="60" type="password" name="old_password"></td></tr>
<tr><td>New Password:</td><td><input size="60" type="password" name="new_password"></td></tr>
<tr><td>Repeat New Password:</td><td><input size="60" type="password" name="confirm_password"></td></tr>
<tr>
<td align="left"><input type="reset" value="Reset"></td>
<td align="right"><input type="submit" value="Save Password"></td>
</tr>
</table>
</form>
