<?php
/**
 * tools for showing different types of calendars
 */
class Calendar {
	const DEFCAL = 'tools/calendars/month.tpl';
	public static $calendar;
	public static $first;
	public static $firstepoch;
	public static $nextdate;
	public static $prevdate;
	public static $startdate;
	public static $startepoch;

	public static function showmonth($template=self::DEFCAL) {
		View::assign('content',self::fetchmonth($template));
		View::display('wrapper.tpl');
	}

	public static function fetchmonth($template=self::DEFCAL) {
		self::$calendar = 'month';
		self::setdates();
		View::assign('weeks', range(0,5));
		# TODO need a better way to generate the days of the week based on locale
		$dow = 0;
		foreach (array('Mon','Tue','Wed','Thu','Fri','Sat','Sun') as $d) {
			$days[++$dow] = $d;
		}
		View::assign('days', $days);
		return View::fetch($template);
	}

	public static function setdates() {
		self::$startdate = self::startdate();
		self::$first = preg_replace('#-\d+$#','-01',self::$startdate);
		self::$firstepoch = strtotime(self::$first);
		self::$startepoch = strtotime(self::$startdate);
		self::$prevdate = date('Y-m-d',strtotime(self::$startdate." - 1 month"));
		self::$nextdate = date('Y-m-d',strtotime(self::$startdate." + 1 month"));
		View::assign('prevdate',self::$prevdate);
		View::assign('nextdate',self::$nextdate);
		View::assign('startday',date('j',self::$startepoch));
		View::assign('startdate',self::$startdate);
		View::assign('eom', ($eom = date('t',self::$startepoch)));
		View::assign('enddate', preg_replace('#-\d+$#',"-$eom",self::$startdate));
		View::assign('firstdow', date('N',self::$firstepoch));
		View::assign('mon', date('m',self::$startepoch));
		View::assign('month', date('F',self::$startepoch));
		View::assign('year', date('Y',self::$startepoch));
	}

	public static function startdate() {
		$date = self::r('startdate');
		if (!Check::isdate($date)) {
			$date = sprintf(
				'%04d-%02d-%02d',
				self::r('Date_Year'),
				self::r('Date_Month'),
				self::r('Date_Day')
			);
		}
		if (!Check::isdate($date)) $date = date('Y-m-d');
		return $date;
	}
	# quiet warning messages
	public static function r($key) {
		return isset($_REQUEST[$key]) ? $_REQUEST[$key] : null;
	}
}

