<?php
/**
 * example of a page or set of pages that require login
 */
class Restricted extends Controller {
	public function execute() {

		# $ldata has the user record
		# you can use this info to fine tune the response logic
		# in this case we accept any valid login, if not we display the login form
		if ($this->ldata = Login::check()) {
			$this->id = $_REQUEST['numbered_id'];
			$this->n = new Numbered;

			$this->doable(array(
				'save' => 'savenote',
				'confirmdelete' => 'confirmdeletenote',
				'delete' => 'deletenote',
			));
			$this->doaction($this->actions[1]);

			if ($this->id) $this->input = $this->n->getone($this->id);
			else $this->input['created'] = date('Y-m-d H:i:s');

			View::assign('schema',$this->n->schema);
			View::display('restricted.tpl');

		} else {
			View::display('tools/login.tpl');
		}
	}

	protected function savenote() {
		if (!$this->id) {
			$_REQUEST['email'] = $this->ldata['login'];
			$this->n->ins($_REQUEST);
			$this->id = $this->n->getid();
		} else {
			$this->n->upd($this->id,$_REQUEST);
		}
		if (!$this->n->err()) {
			View::assign('topmsg',"saved {$this->id}");
			Entity::setpageidhowmany('numbered');
		}
	}

	protected function confirmdeletenote() {
		$this->flag('note_to_delete',$this->id);
		View::assign('confirm',"Really delete note?");
		View::assign('action','restricted/delete');
		View::assign('submit','delete');
		View::display("tools/confirm.tpl");
		exit;
	}

	protected function deletenote() {
		$this->id = $this->delflag('note_to_delete');
		$this->n->del($this->id);
		Entity::setpageidhowmany('numbered');
		View::assign('confirm',"Note {$this->id} deleted!");
		View::display("tools/confirm.tpl");
		exit;
	}
}

