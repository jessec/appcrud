<?php

function pQgSelect () {

	global $pQgCfgFields, $pQgTable, $pQgTableFields;

	if (PQG_LOG) pQgLog("pQgSelect-start: |$fields|$sql|");

	$fields = pQgOptions ('select');

	if (!$fields or $fields == '*')
		$qFields =  $rFields = $pQgCfgFields [$pQgTable];
	else
		list ($qFields, $rFields) = pQgSplitFields($fields);

	$fieldList = implode(',', $pQgCfgFields [$pQgTable] );

	$records = pQgQuery($fieldList);

	$array = array();

	foreach ( $records as $k1 => $record ) {
		
		foreach($pQgCfgFields [$pQgTable] as $k2 => $field)
			$result[$field] = $record[$k2];
			
		foreach($rFields as $k2 => $field)
			if ( isset($pQgTableFields[$field]))
				$array[$k1] [$field] = $result[$qFields[$k2]];
			else 
				$array[$k1] [$field] = pQgGetValue($result, $field, $qFields[$k2], 'PHP');

	}

	if (PQG_LOG) pQgLog("pQgSelect-end: ");

	return $array;

}

function pQgSplitFields($fields) {

	if (PQG_LOG) pQgLog("pQgSplitFields-start: $fields");

	$save = pQgSaveFields($fields);
	
	$iFields = explode(',', $save);

	$qFields = $rFields = array();
	
	foreach($iFields as $k2 => $field) {
		
		$field = trim(pQgSaveFieldsUndo($field));
		
		if ( stristr($field, ' as ') ) {

			$as = stripos($field, ' as ');
			
			$source = trim(substr($field, 0, $as));
			$alias = trim(substr($field, $as+4));
			
			if (PQG_CHECK and !$source )
				pQgError("Incorrect rename of field with AS, field not found: $fields");

			if (PQG_CHECK and !$alias )
				pQgError("Incorrect rename of field with AS, alias not found: $fields");
				
			$qFields[] = trim($source);
			$rFields[] = trim($alias);
			
		}
		
		else {
			
			$qFields[] = $field;
			$rFields[] = $field;
			
		}
		
	}

	if (PQG_LOG) pQgLog("pQgSplitFields-end: qFields: |" . implode('|', $qFields) . '|');
	if (PQG_LOG) pQgLog("pQgSplitFields-end: rFields: |" . implode('|', $rFields) . '|');
	
	return array($qFields, $rFields);

}

?>