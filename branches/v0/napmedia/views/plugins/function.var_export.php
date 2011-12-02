<?php
/**
 * run the php var_export function and do some basic formatting to the output
 */
function smarty_function_var_export($params,&$smarty) {
	return "<pre>".var_export($params['var'],true)."</pre>\n";
}
