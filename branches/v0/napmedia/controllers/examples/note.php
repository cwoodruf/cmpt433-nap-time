<?php
/**
 * basic note display
 */
class Note extends Controller {
	public function execute() {
		if ($ldata = Login::check()) {
			$view = array(
				'email' => $ldata['login'],
				'numbered_id' => $_REQUEST['numbered_id'],
			);
			Run::me('Viewers','ins',$view);
		}
		if (!Check::digits($id = $_REQUEST['numbered_id'])) {
			View::assign('error',"Invalid note id!");
			View::display('home.tpl');
		}
		# "Run::me" and "Run::cached" do the "singleton" pattern 
		# ie: turn objects back into functions (sort of)
		View::assign('note',Run::me('Numbered','getone',$id));
		View::assign('viewers',
			Run::me(
				'Viewers', # object
				'getall', # method to run
				# arguments for getall (getall can be run without arguments):
				array("where numbered_id='%u'",$id), # where query for getall
				'email' # getall can accept a field list, this can be an array
			)
		);
		# you can access the created object later. Example:
		# print Run::$o['Viewers']->query()."<br>\n";
		# if you set Run::$refresh to true saved data is cleared
		View::display('note.tpl');
	}
}

