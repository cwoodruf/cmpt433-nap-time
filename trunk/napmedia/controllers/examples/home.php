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
		// you can also just use print statements here
		// try adding action=home/{some text}
		View::assign('name',htmlentities($this->actions[1]));
		// this will insert the contents of home.tpl into wrapper.tpl
		// you can change the wrapper by adding a template name 
		// as a second argument to this function
		View::wrap('home.tpl');
	}
}

