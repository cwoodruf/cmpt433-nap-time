<?php
class Loginform extends Controller {
	public function execute () {
		$ldata = Login::check();
		# ajaxcheck is the quick check done in the background from the login form
		if ($this->actions[1] == 'ajaxcheck') {
			if (is_array($ldata)) {
				print 'OK';
				exit;
			} else {
				if (!QUIET) print ' Not found / bad password. '; 
				exit;
			}
		}
		if (is_array($ldata)) {
			$_REQUEST['action'] = '';
			require('index.php');
			return;
		}

		$this->flag('login',true);
		View::wrap('tools/login.tpl');
	}
}

