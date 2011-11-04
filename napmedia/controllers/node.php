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
class Node extends Controller {
	/**
	 * required function for controllers
	 */
	public function execute() {
		$this->doable(array(
			'add' => 'add',
			'del' => 'del',
			'ls' => 'ls',
			'status' => 'status',
			'content' => 'content',
			'default' => 'form',
		));
		$this->doaction($this->actions[1]);
	}
	protected function content() {
		$all = Nap::loadip($_REQUEST['ip']);
		View::assign('ip',htmlentities($_REQUEST['ip']));
		View::assign('all',$all);
		View::wrap('nodeedit.tpl');
	}
	protected function form() {
		View::assign('ips',Nap::listips());
		View::wrap('node.tpl');
	}
	protected function add() {
		$ip = $_REQUEST['ip'];
		$desc = htmlentities($_REQUEST['desc']);
		if (Check::isip($ip)) {
			$ipdata = Nap::loadip($_REQUEST['ip']);
			$ipdata['desc'] = $desc;
			$ret = Nap::saveip($_REQUEST['ip'], $ipdata);
			if ($ret === false) {
				View::assign('error',"Error saving data!");
			} else {
				View::assign('topmsg',"Saved data");
			}
		}
		$this->form();
	}
	protected function del() {
		if (is_array(($nodes=$_REQUEST['delnode']))) {
			foreach ($nodes as $ip => $value) {
				if (!$value) continue;
				if (Nap::delip($ip)) {
					$goodip[] = $ip;
				} else {
					$badip[] = $ip;
				}
				if (count($goodip)) 
					View::assign('topmsg',"Deleted ips ".implode(", ", $goodip));
				if (count($badip)) 
					View::assign('error',"Error deleting ips ".implode(", ",$badip));
			}
		} else {
			View::assign('topmsg',"Nothing selected for deletion");
		}
		$this->form();
	}
	protected function ls() {
		header('content-type: text/plain');
		if (($ip = $_REQUEST['ip'])) {
			$i = Nap::loadip($ip);
			if (!$i) $i = $_SERVER;
			else $i = array_merge($i,$_SERVER);
			Nap::saveip($ip,$i);
		}
		print Nap::ipsstr();
	}
	protected function status() {
		$ips = Nap::listips();
		foreach ($ips as $ip => $data) {
			$responses[$ip] = Nap::contact($ip,'ping');
		}
		View::assign('ips',$ips);
		View::assign('responses',$responses);
		$this->form();
	}
}

