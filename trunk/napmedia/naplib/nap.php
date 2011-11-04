<?php
class Nap {
	private static $pwfile = 'naplib/.nappw';
	private static $find = '/usr/bin/find';
	private static $sha1sum = '/usr/bin/sha1sum';
	public static $sumsfile = 'files/sums.txt';

	public static function delip($ip) {
		if (!Check::isip($ip)) return false;
		return rename("nodes/$ip.php","nodes/$ip-".date('YmdHis').".php");
	}
	public static function saveip($ip,$data) {
		if (!Check::isip($ip)) return false;
		return file_put_contents("nodes/$ip.php", serialize($data));
	}
	public static function loadip($ip) {
		if (!Check::isip($ip)) return false;
		if (!is_file("nodes/$ip.php")) return false;
		return unserialize(file_get_contents("nodes/$ip.php"));
	}
	public static function listips() {
		foreach (glob('nodes/[0-9]*.[0-9]*.[0-9]*.[0-9]*.php') as $ipfile) {
			$ip = preg_replace('#^nodes/(.*).php$#',"$1",$ipfile);
			if (!Check::isip($ip)) continue;
			$ips[$ip] = unserialize(file_get_contents($ipfile));
		}
		return $ips;
	}
	public static function ipsstr() {
		$ips = self::listips();
		if (!is_array($ips)) return;
		foreach ($ips as $ip => $i) {
			$txt .= "$ip {$i['desc']}\n";
		}
		return $txt;
	}
	private static function password() {
		@include(self::$pwfile);
		return $NapPASSWORD;
	}
	public static function contact($ip,$cmd,$cgi) {
		return file_get_contents("http://$ip/p2p/$cmd.cgi?id=".Nap::password()."&$cgi");
	}
	public static function sums() {
		if (!is_file(self::$sumsfile)) {
			$findcmd = self::$find." -name '*.mp3'";
			$filesfound = `$findcmd`;
			if ($filesfound) {
				foreach (explode("\n", $filesfound) as $file) {
					if (!$file) continue;
					$content = file_get_contents($file);
					if (!$content) continue;
					$sums[sha1($content)]['files'] .= ":$file";
					$sums[sha1($content)]['size'] = filesize($file);
				}
				file_put_contents(self::$sumsfile,serialize($sums));
			}
		}
		if (!is_file(self::$sumsfile)) return false;
		return unserialize(file_get_contents(self::$sumsfile));
	}
	public static function mergesums($files) {
		$sums = self::sums();
		if (!$sums) $sums = array();
		if (!is_array($files)) return false;
		foreach ($files as $file => $sum) {
			if (preg_match("#^$file$#m",$sums[$sum['sum']]['files'])) continue;
			$sums[$sum['sum']]['files'] .= ":$file";
		}
		return file_put_contents("files/sums.txt", serialize($sums));
	}
	public static function sumsstr() {
		$sums = self::sums();
		foreach ($sums as $sum => $s) {
			$sumsstr .= "$sum {$s['size']} {$s['files']}\n";
		}
		return $sumsstr;
	}
}
