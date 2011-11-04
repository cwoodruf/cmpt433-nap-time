<?php
/**
 * basic login creation form - customize this to work with your own logins
 */
class Register extends Controller {
	private static $LOGINMODEL = LOGINMODEL;

	// change these to match the appropriate fields in your logins table
	private static $USERNAME = 'email';
	private static $OLDUSERNAME = 'oldemail'; // for changing a user name
	private static $PASSWORD = 'password';

	// modify these to work with your specific login requirements 
	public function userchecker($username) {
		return Check::isvar($username) or Check::isemail($username);
	}

	public function pwchecker($password) {
		return Check::validpassword($password);
	}

	/**
	 * main function that manages registrations
	 */
	public function execute() {
		$this->schema = $this->r()->schema;

		$this->doable(array(
			'save' => 'savelogin',
			'edit' => 'editlogin',
		));
		$this->doaction($this->actions[1]);

		View::wrap('tools/register.tpl');
	}

	/**
	 * save a login
	 */
	protected function savelogin() {

		if (!($this->userchecker($this->username()))) {
			View::assign('errors',"Invalid email entered!");
			$this->input = $_REQUEST;
			return;
		}

		if (!($this->pwchecker($_REQUEST[self::$PASSWORD]))) {
			View::assign('errors',"Invalid password entered!");
			$this->input = $_REQUEST;
			return;
		}

		$_REQUEST[self::$PASSWORD] = Login::encode($_REQUEST[self::$PASSWORD]);
		if ($_REQUEST[self::$OLDUSERNAME]) {
			$this->r()->upd($_REQUEST[self::$OLDUSERNAME], $_REQUEST);
		} else {
			$this->r()->ins($_REQUEST);
		}

		$this->input = $this->r()->getone($this->username());
		$this->input[self::$PASSWORD] = '';
		View::assign('topmsg',"saved ".htmlentities($this->username()));

		// in this case we are going right back to the edit form so this makes sense
		$this->editlogin();
	}

        /**
         * we edit based on who you are logged in as in this case
         * if you'd want to edit someone else's login you'd have to get that one
	 */
	protected function editlogin() {
		$ldata = Login::check();
		if ($ldata['login'] == $this->username()) {
			$this->input = Run::me(self::$LOGINMODEL,'getone',$ldata['login']);
			$this->input[self::$PASSWORD] = '';
			$this->hidden[self::$OLDUSERNAME] = $ldata['login'];
		}
	}

	public function username() {
		if (isset($this->username)) return $this->username;
		$this->username = $_REQUEST[self::$USERNAME];
		return $this->username;
	}

	/**
         * get or create a reference to the login model
         */
	protected function r() {
		if (isset($this->r)) return $this->r;
		$this->r = new self::$LOGINMODEL;
		return $this->r;
	}
}

