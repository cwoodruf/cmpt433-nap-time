<?php
/**
 * save url info when logging in - used in the login.tpl template
 * the $login smarty var is set as a side effect 
 */
function smarty_function_login_params($params,&$smarty) {
	$vars = array_merge($_GET,$_POST);
	foreach ($vars as $key => $val) {
		if (preg_match('#^(login|password|cache|app|callback)$#',$key)) continue;
		$hidden .= "<input type=hidden name=\"$key\" value=\"$val\">\n";
	}
	$smarty->assign('login',htmlentities($vars['login']));
	return $hidden;
}

