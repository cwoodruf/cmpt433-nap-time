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
		global $Nap_bridgepid, $Nap_bridge, $Nap_bridgelog;
		# wait between restarts ...
		while (time() - filemtime($Nap_bridgepid) < 10) {
			sleep(1);
		}
		exec("killall napbridge.pl");
		exec("echo `/bin/date` >> $Nap_bridgelog");
		$cwd = getcwd();
		exec("$Nap_bridge -v -P'$cwd/naptime/password' >> $Nap_bridgelog 2>&1 &");
		View::assign('topmsg',"$Nap_bridge restarted");
		View::assign('bridgestatus',$this->bridgestatus());
		# waiting after seems to help with reporting the status below
		sleep(2);
		View::wrap('bridgestatus.tpl');
	}
	protected function bridgestatus() {
		global $Nap_bridgepid, $Nap_bridge, $Nap_bridgelog;
		$cwd = getcwd();
		$pid = trim(file_get_contents($Nap_bridgepid));
		if (preg_match("#\b$pid\b.*$Nap_bridge#",`ps -ef`)) {
			return "$Nap_bridge running as $pid";
		}
		return "$Nap_bridge is not running $pid";
	}
	protected function form() {
		View::assign('bridgestatus',$this->bridgestatus());
		View::assign('nodes',Nap::ipsstr());
		View::assign('files',Nap::sumsstr());
		View::wrap('home.tpl');
	}
}

