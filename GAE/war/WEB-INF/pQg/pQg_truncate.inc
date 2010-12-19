<?php

function pQgTruncate () {

	global $pQgPMF, $pQgClass;

	$pQgPMF->deletePersistentAll($pQgPMF->newQuery("SELECT FROM $pQgClass")->execute());
	
}
	
?>