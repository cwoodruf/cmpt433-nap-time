<?php
class Logout extends Controller {
	public function execute () {
		Login::logout();
		// change the below to fit your site 
		$defcontroller = DEFCONTROLLER;
		$context = new $defcontroller;
		$context->execute();
	}
}

