<?php
require_once('lib/init.php');

list($controller,$actions) = Controller::init();

require_once(Controller::path($controller));
$context = new $controller($actions);
$context->execute();

