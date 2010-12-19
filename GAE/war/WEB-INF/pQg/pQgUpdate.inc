<?php

function pQgUpdate() {

	if (PQG_LOG) pQgLog("pQgUpdate-start: $sql");
	
	pQgDeleteUpdateLimit();

	$objects = pQgQuery($sql);

	$fields = pQgOptions ('set') ;	

	$combine = pQgSetFields($fields);
	
	$count = count($objects);
	
	for ( $i=0; $i < $count; $i++ )
		pQgUpdateObject( $objects[$i], $combine );

	pQgClose();		// $connection->flush();  // Does not work :-(
	pQgOpen();

	if (PQG_LOG) pQgLog("pQgUpdate-end: $count");
	
	return $count;

}
	
?>