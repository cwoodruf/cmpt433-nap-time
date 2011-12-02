<div>
{literal}
<form id="form_login" action="index.php" method="get"
      onsubmit="
$.get(
	'index.php', 
	{ 
		action: 'loginform/ajaxcheck', 
		login: login.value, 
		password: password.value 
	},
	function (data) {
		if (data == 'OK') {
			window.location = 'index.php?action=loginform';
		} else {
			alert('Login failed. '+data);
		}
	}
); return false;"
>
{/literal}
{login_params}
<table cellpadding="3" cellspacing="0" border="0">
<tr><td><b>Login:</b></td>
    <td><input name="login" size="64" maxlength="64" value="{$login}" />
    <script type="text/javascript">document.getElementById("form_login").login.focus()</script></td></tr>
<tr><td><b>Password:</b></td>
    <td><input type="password" name="password" size="64" maxlength="64" /></td></tr>
<tr><td><input type="reset" value='Reset' /></td>
    <td align="right"><input type="submit" value="Log In" /></td></tr>
</table>
</form>
</div>
