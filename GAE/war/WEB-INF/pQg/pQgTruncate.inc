<?php

function pQgTruncate () {

	global $pQgPMF, $pQgClass;

	if (PQG_JDO) $pQgPMF->deletePersistentAll($pQgPMF->newQuery("SELECT FROM $pQgClass")->execute());
	
}
	
?>