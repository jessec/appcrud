<?php
include dirname(__FILE__).'/actions/ActionController.php';
session_start();




$ctr = new ActionController;
$ctr->setParam($_POST,$_GET,$_SESSION);
$log = $ctr->getMessages();
$res = $ctr->getResult();

echo '<pre>';
print_r($log);
print_r($res);





