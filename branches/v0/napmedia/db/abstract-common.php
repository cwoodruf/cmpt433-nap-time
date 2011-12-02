<?php
/*
---------------------------------------------------------------
Author Cal Woodruff cwoodruf@gmail.com
Licensed under the Perl Artistic License version 2.0
http://www.perlfoundation.org/attachment/legal/artistic-2_0.txt
---------------------------------------------------------------
*/

/**
 * class to define entities: 
 * tables that have stand alone records 
 * these tables are assumed to only have one field that is the key
 * you can define this key in the $schema array or simply use {table name}_id 
 */
class Entity extends AbstractDB {
	public $table;
	public $schema;
	public $dbname;
	public $dbhost;
	public $primary;
		
	public function __construct($db,$schema,$tb) {
		parent::__construct($db);

		$this->dbname = $db['db'];
		$this->dbhost = $db['host'];

		if ($tb and is_array($schema)) {
			$this->table = $tb;
			$this->schema = $schema;
			$this->findprimary();
		}
	}

	public function findprimary() {
		# note that for some tables there may not be a primary key
		if (isset($this->schema['PRIMARY KEY'])) {
			$this->primary = $this->schema['PRIMARY KEY'];
			unset($this->schema['PRIMARY KEY']);

		} else if (isset($table) and isset($this->schema[$table.'_id'])) {
			$this->primary = $table.'_id';

		} else {
			foreach ($this->schema as $field => $fdata) {
				if ($fdata['key']) {
					$this->primary = $field;
					break;
				}
			}
		}
	}

	public function ins($data) {
		try {
			if (!preg_match('#^\w+$#', $this->table)) 
				throw new Exception("missing valid table name in ins!");
			$idata = array();
			foreach ($this->schema as $field => $fdata) {
				if ($this->isauto($fdata)) continue;
				# if ($field == 'PRIMARY KEY') continue;
				$this->check($fdata,$data[$field],$field);
				if (!isset($data[$field])) continue;
				$idata[$field] = $this->quote(
						$this->modify($fdata,$data[$field]),"'"
				);
			}
			$this->insert($idata);
			return $this->result;

		} catch(Exception $e) {
			$this->err($e);
			if (!QUIET) die($this->err());
			return false;
		}
	}

	public function upd($id,$data) {
		try {
			if (empty($this->primary)) 
				throw new Exception("no primary key defined!");
			if (!preg_match('#^\w+$#', $this->table)) 
				throw new Exception("missing valid table name in upd!");
			$udata = array();
			foreach ($this->schema as $field => $fdata) {
				if (!isset($data[$field])) continue;
				if ($fdata['key']) continue;
				$this->check($fdata,$data[$field],$field);
				$udata[] = "$field=".$this->quote(
						$this->modify($fdata,$data[$field]),"'"
				);
			}
			$update = "update {$this->table} set ".implode(",", $udata)." where {$this->primary}='%s'";
			$this->run($update,$id);
			return $this->result;

		} catch(Exception $e) {
			$this->err($e);
			if (!QUIET) die($this->err());
			return false;
		}
	}

	public function del($id) {
		try {
			if (empty($this->primary)) 
				throw new Exception("no primary key defined!");
			if (!preg_match('#^\w+$#', $this->table)) 
				throw new Exception("missing valid table name in del!");
			$this->run("delete from {$this->table} where {$this->primary}='%s'", $id);
			return $this->result;
		} catch(Exception $e) {
			$this->err($e);
			if (!QUIET) die($this->err());
			return false;
		}
	}

	public function getall($criterion=null,$fields=null) {
		try {
			if (!preg_match('#^\w+$#', $this->table)) 
				throw new Exception("missing valid table name in getall!");
			$fieldstr = self::mk_fieldstr($fields);
			$this->run_criterion("select $fieldstr from {$this->table}",$criterion);
			return $this->resultarray();
		} catch (Exception $e) {
			$this->err($e);
			if (!QUIET) die($this->err());
			return false;
		}
	}

	protected static function mk_fieldstr($fields) {
		// deliberately being open ended here
		// this should not use unchecked user input
		if (is_array($fields)) {
			$fieldstr = implode(',', $fields);
		} else if (is_string($fields)) {
			$fieldstr = $fields;
		} else {
			$fieldstr = '*';
		}
		return $fieldstr;
	}

	public function howmany($criterion=null) {
		try {
			if (!preg_match('#^\w+$#', $this->table)) 
				throw new Exception("missing valid table name in upd!");
			$this->run_criterion("select count(*) as howmany from {$this->table}",$criterion);
			$row = $this->getnext();
			$this->free();
			return $row['howmany'];
		} catch (Exception $e) {
			$this->err($e);
			if (!QUIET) die($this->err());
			return false;
		}
	}
	
	protected function run_criterion($select,$criterion) {
		if (is_array($criterion)) {
			$criterion[0] = "$select {$criterion[0]}";
			call_user_func_array(array($this,"run"),$criterion); 
		} else {
			$this->run("$select $criterion");
		}
	}

	public function getone($id,$fields=null) {
		try {
			if (empty($this->primary)) 
				throw new Exception("no primary key defined!");
			if (!preg_match('#^\w+$#', $this->table)) 
				throw new Exception("missing valid table name in getone!");
			$fieldstr = self::mk_fieldstr($fields);
			$this->run("select $fieldstr from {$this->table} where {$this->primary}='%s'", $id);
			$row = $this->getnext();
			$this->free();
			return $row;

		} catch (Exception $e) {
			$this->err($e);
			if (!QUIET) die($this->err());
			return false;
		}
	}

	public function modify($fdata,$value) {
		if (!isset($fdata['modifier'])) return $value;
		if (is_array($fdata['modifier'])) {
			$modifiers = $fdata['modifier'];
			foreach ($modifiers as $class => $method) {
				if (method_exists($class,$method)) {
					$value = call_user_func_array(array($class,$method),array($value));
					return $value;
				}
			}
			throw new Exception("Could not find valid modifier method!");
		} else {
			$modifier = $fdata['modifier'];
			if (function_exists($modifier)) {
				$value = $modifier($value);
				return $value;
			} else {
				throw new Exception("Could not find modifier: $modifier.");
			}
		}
	}

	# check a value before inserting or updating into the database
	public function check($fdata,$value,$field) {
		if (!isset($fdata['checker'])) return;
		if (is_array($fdata['checker'])) {
			$checkers = $fdata['checker'];
			foreach ($checkers as $class => $method) {
				if (method_exists($class,$method)) {
					if (call_user_func_array(array($class,$method),array($value))) return;
					else throw new Exception("Invalid value '$value' for $field!");
				}
			}
			throw new Exception("Could not find valid checker method for $field!");
		} else {
			$checker = $fdata['checker'];
			if (function_exists($checker)) {
				if (!$checker($value)) {
					throw new Exception("Invalid value '$value' for $field!");
				}
			} else {
				throw new Exception("Could not find checker: $checker for $field.");
			}
		}
	}

	public function isauto($fdata) {
		if (isset($fdata['auto']) and $fdata['auto']) return true;
	}

	public function iskey($field,$fdata) {
		if ($field == 'PRIMARY KEY') return true;
		if ($fdata['key']) return true;
		return false;
	}

	/** 
	 * save an error message or print it
	 */
	public function err(Exception $e = null) {
		if ($e != null) {
			$this->error = $e->getMessage();
			if (!QUIET) die($this->error."<br>\n".$this->query);
		}
		return $this->error;
	} 
	
	public function query() {
		return $this->query;
	}

	/**
	 * function to manage paging db results
	 */
	# the backbone for pagination - uses getall on the supplied model - 
	# ie won't work with other types of models - this is bad as we'd like a general way to do this
	# alternatively check what model is and then do something appropriate
	public static function getpageid($pageid,$field=null) {
		if ($field and isset($_SESSION['paged'][$pageid][$field])) 
			return $_SESSION['paged'][$pageid][$field];
		return $_SESSION['paged'][$pageid];
	}

	public static function setpageid($pageid,$field,$value) {
		if ($field and isset($_SESSION['paged'][$pageid][$field])) 
			$_SESSION['paged'][$pageid][$field] = $value;
		return $_SESSION['paged'][$pageid][$field];
	}

	public static function setpageidhowmany($pageid) {
		if (isset($_SESSION['paged'][$pageid]['model'])) {
			$model = $_SESSION['paged'][$pageid]['model'];
			$criterion = $_SESSION['paged'][$pageid]['criterion'];
			return $_SESSION['paged'][$pageid]['howmany'] = 
				self::get_criterion_howmany($model,$criterion);
		}
	}

	public static function get_criterion_howmany($model,$criterion) {
		if (method_exists($model,$criterion)) {
			$ary = $model->$criterion(null,array("count(*) as n"));
			$howmany = $ary[0]['n'];
		} else {
			$howmany = $model->howmany($criterion);
		}
		return $howmany;
	}

	public static function delpageid($pageid) {
		$pagerdata = $_SESSION['paged'][$pageid];
		unset($_SESSION['paged'][$pageid]);
		return $pagerdata;
	}

	public static function getpage($pageid,$model,$offset=0,$limit=10,$criterion=null,$fields=null) {

		if (!is_object($model)) return "Model is not an object!";

		if (!$pageid) return "Bad page id!";
		if (is_array($_SESSION['paged'][$pageid])) {
			# probably should redo this every time but this saves some resources
			$howmany = $_SESSION['paged'][$pageid]['howmany'];
			if (!isset($criterion)) 
				$criterion = $_SESSION['paged'][$pageid]['criterion'];
			if (!isset($fields)) 
				$fields = $_SESSION['paged'][$pageid]['fields'];
		} else {
			$howmany = self::get_criterion_howmany($model,$criterion);
		}

		if (!Check::digits($limit)) return "limit is not a number!";
		if (!Check::digits($offset)) $offset = 0;

		if (method_exists($model,$criterion)) {
			$rows = $model->$criterion("limit $limit offset $offset",$fields);
		} else {
			$rows = $model->getall("$criterion limit $limit offset $offset",$fields);
		}

		$_SESSION['paged'][$pageid] = array(
			'criterion' => $criterion,
			'fields' => $fields,
			'howmany' => $howmany,
			'limit' => $limit,
			'offset' => $offset,
			'model' => $model,
		);
		return array('limit' => $limit, 'howmany' => $howmany, 'offset' => $offset, 'rows' => $rows);
	}

}

/**
 * class to define relations: ie tables that connect other tables together
 * with relations you need to keep track of more than one table so 
 * we need to add some functionality to handle that gracefully
 */
class Relation extends Entity {

	public function __construct($db,$schema,$table) {
		parent::__construct($db,$schema,$table);
	}

	/**
	 * update delete and select operations should be defined for each relation
	 * since these are identified by more than one value $id is necessarily an array
	 */
	public function upd($id,$data) { 
		try {
			$args = $this->splitid($id);
			$key = array_shift($args);
			$set = array();
			$vals = array();
			foreach ($this->schema as $field => $fdata) {
				if ($this->iskey($field,$fdata)) continue;
				if (!isset($data[$field])) continue;
				$this->check($fdata,$data[$field],$field);
				$set[] = "$field='%s'";
				$vals[] = $this->modify($fdata,$data[$field]);
			}
			$query = "update {$this->table} set ".implode(",", $set)." where $key";
			$valskeys = array_merge(array($query),$vals,$args);
			call_user_func_array( array($this,'run'), $valskeys );
			return $this->result;

		} catch (Exception $e) {
			$this->err($e);
			if (!QUIET) die($this->err());
			return false;
		}
	}

	public function del($id) { 
		try {
			$args = $this->splitid($id);
			$args[0] = "delete from {$this->table} where {$args[0]}";
			call_user_func_array( array($this,'run'), $args );
			return $this->result;

		} catch (Exception $e) {
			$this->err($e);
			if (!QUIET) die($this->err());
			return false;
		}
	}

	public function getone($id,$fields=null) { 
		try {
			$args = $this->splitid($id);
			$fieldstr = self::mk_fieldstr($fields);
			$args[0] = "select $fieldstr from {$this->table} where {$args[0]}";
			call_user_func_array( array($this,'run'), $args );
			$row = $this->getnext();
			$this->free();
			return $row;

		} catch (Exception $e) {
			$this->err($e);
			if (!QUIET) die($this->err());
			return false;
		}
	}

	/**
	 * process an id array in such a way that its easier to use the run method with it
	 * for basic queries 
	 * @param $id - array of primary key field names and values to look for
	 * @return an array with the field string and key values as a single array
	 *         the field string is always the 0th element in the array
	 */
	public function splitid($id) {
		if (!is_array($id)) 
			throw new Exception("splitid: relation id is not an array");
		if (!is_array($this->primary)) 
			throw new Exception("splitid: primary key is not array");

		foreach ($this->primary as $field => $table) {
			if (empty($id[$field])) 
				throw new Exception("splitid: missing $field in id");
			$fields[] = $field."='%s'";
			$ids[] = $id[$field];
		}
		return array_merge( array( '('.implode(' and ', $fields).')' ), $ids );
	}
}

