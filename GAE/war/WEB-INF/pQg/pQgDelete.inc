<?php

function pQgDelete () {

	global $pQgPMF;

	if (PQG_LOG) pQgLog("pQgDelete-start:");
	
	pQgDeleteUpdateLimit ();
	
	$objects = pQgQuery();

	foreach ( $objects as $object ) {

		if (PQG_LOG) pQgLog("pQgDelete-delete: Deleted record with primary key: " . pQgGetKey($object) );
		
		if (PQG_JDO) $pQgPMF->deletePersistent($object);

	}
		
	$count = count($objects);
	
	if (PQG_LOG) pQgLog("pQgDelete-end: $count");
	
	return $count;
	
}
	
?>