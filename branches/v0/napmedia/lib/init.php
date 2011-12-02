<?php
@include_once('.settings.php');

# change the following in .settings.php (above)

# debug output
error_reporting(E_ALL & ~E_NOTICE);
if (!defined('QUIET')) {
	if (file_exists('DEBUG') or $_ENV['DEBUG']) {
		define('QUIET',false);
		ini_set('display_errors',true);
	}
} else {
	ini_set('display_errors',false);
}

# for the Login::encode function
if (!defined('SALTFILE')) {
	if (file_exists('.salt.php')) define('SALTFILE','.salt.php');
	else define('SALTFILE',false);
}

#directories
# where the base models are (ie the ones that are automatically made by scripts)
if (!defined('MODELSBASE')) 	define('MODELSBASE','models/base');
# where the model subclasses are (hand written)
if (!defined('MODELSDIR')) 	define('MODELSDIR','models');
# where the controllers are
if (!defined('CONTROLLERSDIR')) define('CONTROLLERSDIR','controllers');
# where the view logic is
if (!defined('VIEWDIR')) 	define('VIEWDIR','views');
# where these libraries are
if (!defined('LIBDIR')) 	define('LIBDIR',dirname(__FILE__));
# where the base db libraries are
if (!defined('DBDIR')) 		define('DBDIR','db');

#components
# parameter to use to determine controller "action"
if (!defined('ACTION')) 	define('ACTION','action');
# default page to show if we don't know what visitor wants to do
if (!defined('DEFCONTROLLER')) 	define('DEFCONTROLLER','home');
# object that manages password retrieval
if (!defined('LOGINMODEL')) 	define('LOGINMODEL','User');
# object that manages login forms
if (!defined('LOGINCONTROLLER')) define('LOGINCONTROLLER','Loginform');
# key in the $_SESSION array for this login - this should be different for each site
if (!defined('LOGINSESSION')) 	define('LOGINSESSION',dirname(__FILE__));

require_once(DBDIR.'/abstract-mysql.php');
require_once(DBDIR.'/abstract-common.php');
require_once(VIEWDIR.'/init.php');
require_once(LIBDIR.'/run.php');
require_once(LIBDIR.'/check.php');
# require that there be something in the input variable when running a Check::* method
Check::$emptyok = false; 
require_once(LIBDIR.'/controller.php');
require_once(LIBDIR.'/pw.php');
require_once(LIBDIR.'/login.php');

function __autoload($class) {
	# changes typeable file names to camel case class names
	# $class = preg_replace('#(?:^|_)(.)#e',"strtoupper($1)",$class);

	if (preg_match('#(.*)(?:Relation|Entity)$#',$class,$m)) {
		$path = MODELSBASE."/".strtolower($m[1]).'_base.php';
		require_once($path);
		return;

	} else if (preg_match('#(.*)DB$#',$class,$m)) {
		$path = MODELSBASE."/".strtolower($m[1]).'_db.php';
		require_once($path);
		return;
	} 

	foreach (array(MODELSDIR,CONTROLLERSDIR,VIEWDIR,LIBDIR) as $dir) {
		$path = "$dir/".strtolower($class).'.php';
		@include_once($path);
	}
}

