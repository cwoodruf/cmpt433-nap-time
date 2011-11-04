<?php
/**
 * grab field data from a model for use in displaying a template
 */
function smarty_function_schema($params,&$smarty) {

	$schema = $params['schema'];
	if (!is_array($schema)) return;
	schema_data($schema);

	if (preg_match('#^\w+$#',$params['assign'])) 
		$output = $params['assign'];
	else $output = 'schema';
	$smarty->assign($output,$schema);
}

/**
 * massage schema table to use defaults defined by view for fields
 */
function schema_data(&$schema) {
	foreach ($schema as $field => $fdata) {
		if ($field == 'PRIMARY KEY') continue;
		switch($fdata['type']) {
		case 'text': 
			if (!$fdata['rows']) $schema[$field]['rows'] = View::DEFROWS;
			if (!$fdata['cols']) $schema[$field]['cols'] = View::DEFCOLS;
		break;
		case 'varchar': 
			unset($size);
			if (!$fdata['size']) $size = View::DEFSIZE;
			if ($fdata['size'] > View::MAXSIZE) $size = View::MAXSIZE;
			if ($size) $schema[$field]['size'] = $size;
		break;
		}
	}
	return $tables;
}

