<?php
/**
 * default controller - customize it
 *
 * you can use the $this->actions array to select which action you want to do
 * use $this->doable(array( ...map of action to callback funcs...));
 * and $this->doaction($this->actions[1]);
 * to handle sub actions such as home/someaction
 *
 * you can display templates with the default smarty view code 
 * View::assign - to assign template variables
 * View::display - display the template
 * View::wrap - display template in a wrapper 
 */
class Home extends Controller {
	/**
	 * required function for controllers
	 */
	public function execute() {
		$this->doable(array(
			'restartbridge' => 'restartbridge',
			'default' => 'form',
		));
		$this->doaction($this->actions[1]);
	}
	protected function restartbridge() {
		exec("killall napbridge.pl");
		exec("echo `/bin/date` >> napbridge.txt");
		exec("naptime/napbridge.pl -v >> napbridge.txt 2>&1 &",$outarray,$response);
		View::assign('topmsg','napbridge.pl restarted');
		$this->form();
	}
	protected function bridgestatus() {
		$cwd = getcwd();
		$pid = trim(file_get_contents("nodes/napbridge.pid"));
		if ($pid and preg_match("#$pid.*napbridge.pl#",`ps -ef`)) {
			return "napbridge.pl running on ".htmlentities($pid);
		}
		return "napbridge.pl is not running $pid";
	}
	protected function form() {
		View::assign('bridgestatus',$this->bridgestatus());
		View::assign('nodes',Nap::ipsstr());
		View::assign('files',Nap::sumsstr());
		View::wrap('home.tpl');
	}
}

