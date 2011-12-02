<?php
/**
 * replaces the singleton idea for doing a one-off run of a function
 * saves the created objects for later reuse - though this can be turned off
 * can also cache the results of a function that is run often
 */
class Run {
	public static $o;
	public static $results;
	public static $error;
	/**
	 * if $refresh is set to true caching is turned off
	 */
	public static $refresh = false;
	/**
	 * singleton equivalent: cache the class unless refresh is set
	 * use Run::cache if you want to save the results for reuse
	 */
	public static function me() {
		try {
			$args = func_get_args();
			$class = array_shift($args);
			$func = array_shift($args);
			if (self::$refresh or !isset(self::$o[$class])) {
				self::$o[$class] = new $class;
			}
			return call_user_func_array(array(self::$o[$class], $func),$args);

		} catch (Exception $e) {
			self::err($e);
			if (!QUIET) die(self::err());
			return false;
		}
	}
	/**
	 * either run something or return a cached set of results
	 * uses sha1 to make the signature
	 */
	public static function cached($class,$func,$args) {
		$sig = $class.'.'.$func.'.'.sha1(serialize($args));

		if (!self::$refresh and isset(self::$results[$sig])) {
			return self::$results[$sig];
		}

		self::$results[$sig] = self::me($class,$func,$args);
		return self::$results[$sig];
	}
	/**
	 * set or return the last error
	 */
	public static function err($e=null) {
		if (isset($e)) $error = $e->getMessage();
		else return $error;
	}
}

