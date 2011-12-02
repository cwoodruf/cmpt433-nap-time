<?php
interface PW {
	public function valid_login($login);
	public function valid_pw($pw);
	public function encode_pw($pw);
	public function get_login($id);
}
