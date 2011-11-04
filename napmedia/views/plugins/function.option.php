<?php
/**
 * make an option for a select box and check if it is selected
 */
function smarty_function_option($params,&$smarty) {
	$instance = $smarty->get_template_vars('this');
	$select = $smarty->get_template_vars('select');

	if (is_object($instance) and $instance->input($select) == $params['value']) {
		$selected = 'selected';
	} else {
		$selected = '';
	}

	return "<option value=\"{$params['value']}\" $selected>{$params['value']}</option>\n";
}

