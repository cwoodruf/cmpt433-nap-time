<?php
/*
---------------------------------------------------------------
Author Cal Woodruff cwoodruf@gmail.com
Licensed under the Perl Artistic License version 2.0
http://www.perlfoundation.org/attachment/legal/artistic-2_0.txt
---------------------------------------------------------------
*/

/**
 * base class for working with mysql 
 * this should be the only place where mysql specific code shows up
 * needs an array $tables with schema information - 
 * this can be generated from the mysql schema information
 */
abstract class AbstractDB {
	public $db;
	public $conn;
	public $schema;
	public $error;
	public $query;
	public $result;
	public $lastid;

	public function __construct($dbdata) {
		$this->connect($dbdata);
	}

	/**
	 * each group of entities should have a common way to connect to the db
	 */
	public function connect($d=null) {
		if (!is_array($d)) $d = $this->db;
		else $this->db = $d;

		$this->conn = mysql_connect($d['host'],$d['login'],$d['pw']);
		if (!$this->conn) throw new Exception("can't connect: ".mysql_error());

		$res = mysql_select_db($d['db'], $this->conn);
		if (!$res) throw new Exception("can't select database: ".mysql_error());

		return $this->conn;
	}

	/**
	 * if we are using a serialized instance of an AbstractDB object
	 * we'll need to check and recreate the connection
	 */
	public function conn() {
		if (!is_resource($this->conn) or !mysql_ping($this->conn)) {
			$this->connect();
		}
		return $this->conn;
	}

	/**
	 * insert update delete and select operations should be defined for each entity
	 */
	public abstract function ins($data);
	public abstract function upd($id,$data);
	public abstract function del($id);
	public abstract function howmany($criterion=null);
	public abstract function getall($criterion=null);
	public abstract function getone($id);

	/**
	 * as insert ignore is mysql specific its defined here
	 */
	public function insert($idata) {
		$insert = "insert ignore into {$this->table} (".implode(",",array_keys($idata)).") ".
				"values (".implode(",",array_values($idata)).")";
		$this->run($insert);
	}

	/**
	 * run allows us to escape query data selectively and then run a query
	 */
	public function run() {
		$args = func_get_args();
		$query = array_shift($args);

		if (count($args)) {
			$query = vsprintf($query,$this->quote($args));
		} 
		$this->query = $query;
		$this->result = mysql_query($this->query,$this->conn());
		if (!$this->result) throw new Exception("query run error: ".mysql_error());
		return $this->result;
	}

	/**
	 * make a basic array of a result set
	 * run should have been run first to create a result set
	 */
	public function resultarray($keep=false) {
		if (!is_resource($this->result)) return false;
		$out = null;
		while ($row = mysql_fetch_assoc($this->result)) {
			$out[] = $row;
		}
		if (!$keep) $this->free();
		return $out;
	}
	/**
	 * run the mysql last_insert_id() function
	 */
	public function getid() {
		$this->run("select last_insert_id() as id");
		$row = $this->getnext();
		$this->free();
		return $this->lastid = $row['id'];
	}
	/**
	 * check for the number of rows returned
	 */
	public function num() {
		if (!is_resource($this->result)) return false;
		return mysql_num_rows($this->result);
	}
	/**
	 * simply get the next row
	 */
	public function getnext() {
		if (!is_resource($this->result)) return false;
		return mysql_fetch_assoc($this->result);
	}
	/**
         * clear a query result
	 */
	public function free() {
		if (!is_resource($this->result)) return false;
		return mysql_free_result($this->result);
	}
	/**
	 * run input through a string cleanup function
	 */
	public function quote($str,$quote='') {
		if (is_array($str)) {
			$quoted = array();
			foreach ($str as $s) {
				$quoted[] = $quote.mysql_real_escape_string($s,$this->conn()).$quote;
			}
			return $quoted;
		}
		return $quote.mysql_real_escape_string($str,$this->conn()).$quote;
	}
	/**
	 * just get the last error
	 */
	public function dberr() {
		return mysql_error();
	}
}

