<?php
require_once(VIEWDIR.'/smarty/Smarty.class.php');
if (!defined('VIEWWRAPPER')) define('VIEWWRAPPER','wrapper.tpl');

# base view class
class View {
	const DEFROWS = 5;
	const DEFCOLS = 60;
	const DEFSIZE = 50;
	const MAXSIZE = 60;
	public static $smarty;
	public static $css;
	public static $js;
	public static $cssfiles;
	public static $jsfiles;
	public static $tplext;

	public static function init($ext='tpl') {
		$smarty = new Smarty();
		$smarty->template_dir = VIEWDIR.'/templates';
		$smarty->compile_dir = VIEWDIR.'/templates_c';
		$smarty->cache_dir = VIEWDIR.'/cache';
		$smarty->plugins_dir[] = VIEWDIR.'/plugins';
		$smarty->plugins_dir[] = dirname(__FILE__).'/smarty/plugins';
		self::$smarty = $smarty;
		self::$tplext = $ext;
	}
	
	public static function assign($name,$value) {
		return self::$smarty->assign($name,$value);
	}

	public static function set($name,$value) {
		return self::$smarty->assign($name,$value);
	}


	public static function wrap($tpl,$wrapper=VIEWWRAPPER) {
		if (self::$smarty->template_exists($wrapper)) {
			self::assign('content',self::fetch($tpl));
			self::display($wrapper);
		} else {
			self::display($tpl);
		}
	}

	public static function display($tpl) {
		self::fixname($tpl);
		return self::$smarty->display($tpl);
	}
	
	public static function fetch($tpl) {
		self::fixname($tpl);
		return self::$smarty->fetch($tpl);
	}

	public static function fixname(&$tpl) {
		if (preg_match("#\.".self::$tplext."#",$tpl)) return;
		$tpl .= ".".self::$tplext;
	}

	public static function addCSS($css) {
		if (!@filetype($css)) $css = "views/css/$css";
		if (!@filetype($css)) return;
		if (self::$cssfiles[$css]) return;
		self::$cssfiles[$css] = true;
		$sitedir = Controller::sitedir();
		self::$css .= "<link rel=\"stylesheet\" type=\"text/css\" href=\"$sitedir/$css\">\n";
	}

	public static function addJS($js) {
		if (!@filetype($js)) $js = "views/js/$js";
		if (!@filetype($js)) return;
		if (self::$jsfiles[$js]) return;
		self::$jsfiles[$js] = true;
		$sitedir = Controller::sitedir();
		self::$js .= "<script type=\"text/javascript\" src=\"$sitedir/$js\" ></script>\n";
	}
}

