<?php
if (!defined('VIEWDIR')) 
	define('VIEWDIR',dirname(__FILE__));

define('SMARTY_DIR',VIEWDIR.'/smarty/');

require_once(VIEWDIR.'/view.php');

View::init();
View::addCSS('main.css');
View::addJS('jquery.js');

