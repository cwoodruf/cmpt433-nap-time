<?php
class Upload extends Controller {
	private $du = '/usr/bin/du';
	private $zipinfo = '/usr/bin/zipinfo';
	private $unzip = '/usr/bin/unzip';
	private $dirmaxsize = 1073741824; # 1 GB

	public function execute() {
		$this->doable(array(
			'save' => 'save',
			'ls' => 'ls',
			'default' => 'form',
		));
		$this->doaction($this->actions[1]);
	}
	protected function form() {
		View::assign('files',Nap::sumsstr());
		View::wrap('upload.tpl');
	}
	protected function save() {
		$uf = $_FILES['mediafile'];
		if ($uf) {
			$dirsize = `{$this->du} -bs files`;
			$dirsize = preg_replace('#\s.*#','',$dirsize);

			if ($uf['error'] > 0) {
				View::assign('error',"Php error uploading {$uf['error']}!");

			} else if (!preg_match('#^application/.*zip#',$uf['type'])) {
				View::assign('error',"Wrong mime type: need a zip file not {$uf['type']}!");

			# this assumes files are fairly compressed
			} else if ($uf['size'] + $dirsize > $this->dirmaxsize) {
				View::assign('error',"Too many files in directory: won't save more!");

			} else {
				$tmpname = "files/".$_SERVER['REMOTE_ADDR'].".zip";
				if (!move_uploaded_file($uf['tmp_name'], $tmpname)) {
					View::assign('error',"Error moving uploaded file to $tmpname!");

				} else {
					$files = `{$this->zipinfo} -1 $tmpname`;
					foreach (explode("\n", $files) as $file) {
						if (!$file or preg_match('#/$#',$file)) continue;
						if (!preg_match('#.mp3#',$file)) {
							$bad .= "$file is not an mp3 file!<br>\n";
							continue;
						}
						$ddir = preg_replace('#(.*)/.*#',"$1",$file);
						if (!is_dir("files/$ddir")) mkdir("files/$ddir",0777,true);
						$dest = "files/$file";
						$cmd = escapeshellcmd("{$this->unzip} -qq -c '$tmpname' '$file'");
						exec("$cmd > ".escapeshellarg($dest));
						if (is_file("files/$file")) {
							$saved[$dest]['sum'] = sha1(file_get_contents($dest));
							$saved[$dest]['size'] = filesize($dest);
							$good .= "saved $dest<br>\n";
						} else {
							$bad .= "error from zip for $dest<br>\n";
						}
					}
					View::assign('error',$bad);
					View::assign('topmsg',$good);
				}
				Nap::mergesums($saved);
#				unlink($tmpname);
			}
		}
		$this->form();
	}
	protected function ls() {
		header('content-type: text/plain');
		print Nap::sumsstr();
	}
}
