<?php 

function pQgRecursion($sql,$cmd='') {

	global $pQgInternal;
	
	if (PQG_LOG) pQgLog("pQgRecursion-start: |$cmd|$sql|");

	foreach($pQgInternal as $field) {
		$data[$field] = $GLOBALS["pQg$field"];
		unset($GLOBALS["pQg$field"]);
	}
	
 	$go = "sql$cmd";
 	
    $return = $go($sql);

	foreach($pQgInternal as $field)	
		$GLOBALS["pQg$field"] = $data[$field];
		
	return $return;

}

?>