<?php

function pQgInsert() {

	if (PQG_LOG) pQgLog("pQgInsert-start:");

	             $set    = pQgOptions    ( 'set'    		);
	             $select = pQgOptionsEnd ( 'select' );
	             $value  = pQgOptions    ( 'values' );
	if (!$value) $value  = pQgOptions    ( 'value'  );

	if (PQG_CHECK and ( ($set and $select and $value) or ($set and $select) or ( $set and $value) or ($select and $value) ) )
		pQgError("Only one of the options VALUE(S), SET or SELECT is allowed.");

	if     ( $set )		 $return = pQgInsertSet($set); 
	elseif ( $value )	 $return = pQgInsertValues($value);	
	elseif ( $select )	 $return = pQgInsertSelect($select);	
	elseif ( PQG_CHECK ) pQgError("Insert needs VALUE(S), SET or SELECT as option.");
	
	if (PQG_LOG) pQgLog("pQgInsert-end: $return");
		
	return $return;
	
}

function pQgInsertSelect($sql) {
	
	if (PQG_LOG) pQgLog("pQgInsertSelect-start: $sql");
	
	$fields  = pQgInsertFields ();
	
	$records = pQgRecursion("SELECT $sql");
	
	foreach($records as $key => $record)
		foreach($record as $key2 => $value)
	  		$insert [$key][] = $value;
	  		
	if ( ! count($fields) and count($records) )
		foreach($records[0] as $key => $value)
			$fields[] = $key;
	
	$return = pQgInsertRecords($fields, $insert, 'ASIS');

	if (PQG_LOG) pQgLog("pQgInsertSelect-end: |$return|");
	
	return $return;
	
}

function pQgInsertFields() {
	
	$key    = pQgGetByWord('insert into') + 1 ;
	
	$fields = pQgOptions ( $key ) ;

	if ($fields) {
		$fields = substr( $fields, 1, -1) ;
		$fields = explode ( ',' , $fields );
	}
	else
		$fields = array();
		
	return $fields;
	
}





function pQgInsertSet ($sql) {

	if (PQG_LOG) pQgLog("pQgInsertSet-start: $sql");
		
	$combine = pQgSetFields($sql); 

	return pQgInsertRecord ($combine);	
		
}

function pQgInsertValues($sql) {
	
	if (PQG_LOG) pQgLog("pQgInsertValues-start: |$sql|");
	
	$start = strpos  ( $sql, '(' );
	$end   = strrpos ( $sql, ')' );

	if (PQG_CHECK and ($start === FALSE or !$end ) )
		pQgError("Value-list not enclosed with (): $sql");
	
	$valuesString = trim(substr($sql, $start+1, ($end-$start)-1));
	
	if (PQG_CHECK and !$valuesString)
		pQgError("No value-list found: $sql");
	
	$valuesStringSave = pQgSaveFields($valuesString);
	$valuesStringSave = $valuesString;
	
	$records = explode(' ) , ( ', $valuesStringSave);
	
	foreach($records as $key => $record) {	

	    if (PQG_LOG) pQgLog("pQgInsertValues-record: $record");
		
		$valuesString = trim(pQgSaveFieldsUndo($record));

		if (PQG_CHECK and !$valuesString)
			pQgError("New record '$key' has an empty value-list");

		$valuesStringSave = pQgSaveFields($valuesString);

		$values = explode(',', $valuesStringSave);

		$inserts[] = $values;
			
	}

	$fields  = pQgInsertFields ();
	
	if ( count($inserts) > 1 ) 
		$return = pQgInsertRecords($fields, $inserts);
	else {
		foreach($fields as $key => $value)
			$combine[ trim($value) ] = pQgSaveFieldsUndo($inserts[0][$key] );	
			$return = pQgInsertRecord($combine);
	}
	
	if (PQG_LOG) pQgLog("pQgInsertValues-end: |$return|");
	
	return $return;
	
}

function pQgInsertRecord ($combine, $option='') {

	global $pQgPMF, $pQgTable;

	if (PQG_LOG) pQgLog("pQgInsertRecord-start: " . count($combine));
	
	$new = new $pQgTable;
	
	pQgUpdateObject($new, $combine, $option);

	if (PQG_JDO) $pQgPMF->makePersistent($new);

	$key = pQgGetKey($new);
	
	if (PQG_LOG) pQgLog("pQgInsertRecord-end: Record added: Table " . $pQgTable . " with primary key $key");
	
	return $key;
	
}

function pQgInsertRecords($fields, $records, $option='') {

	global $pQgPMF, $pQgTable;
	
	if (PQG_LOG) pQgLog("pQgInsertRecords-start:");

	$objects = array();
	
	foreach($records as $key => $values) {	
		
		foreach($values as $key2 => $value)
			$combine[ trim($fields[$key2]) ] = pQgSaveFieldsUndo($value);

		$objects[$key] = new $pQgTable;
	
		pQgUpdateObject($objects[$key], $combine, $option);
 	
	}

	if (PQG_JDO) $pQgPMF->makePersistentAll($objects);
	
	$count = count($records);

	if (PQG_LOG) pQgLog("pQgInsertRecords-end: $count record(s) added");
	
	return array($count);
	
}

?>