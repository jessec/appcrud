<?php

function pQgSelect () {

	global $skip, $pQgCfgFields, $pQgAlias, $pQgTable, $pQgTableFields, $pQgCfgKey, $pQgOnlyKey, $pQgExists;
	
	if (PQG_LOG) pQgLog("pQgSelect-start:");
	
	$fields = pQgOptions ('select');
	
	if ($fields == 'count ( * )' or ( isset($pQgOnlyKey) and $pQgOnlyKey) )
		$qFields = $rFields = array($key);
	elseif (!$fields or $fields == '*') 
		$qFields = $rFields =  $pQgCfgFields[$pQgTable];
	else
		list ($qFields, $rFields) = pQgSplitFields($fields);
	
	$fieldList = implode(',', $pQgCfgFields[$pQgTable]);
	
	$records = pQgQuery($fieldList);

	$count = count($records);
	if (PQG_LOG) pQgLog("pQgSelect-records: $count");
	
	$result = array();
	
	foreach ( $records as $k1 => $record ) {
	
		$tableKey = $record [0];
		
		foreach($pQgCfgFields[$pQgTable] as $k2 => $field)
			$result [$tableKey] [$field] = $record[$k2];

		foreach($rFields as $k2 => $field)
			if ($field <> $qFields[$k2])
				$result [$tableKey] [$field] = pQgGetValue($result[$tableKey], $field, $qFields[$k2], 'PHP');

	}		
		
	$count = count($result);
	if (PQG_LOG) pQgLog("pQgSelect-result: $count");
	
	if ( isset($pQgExists) and is_array($pQgExists) and count($pQgExists) ) {

		$existsLoop = $pQgExists;
		unset($GLOBALS['pQgExists']);
		
		foreach($result as $key => $record) {
			
			foreach($existsLoop as $exists) {

				$sql = $exists['sub'];
				
				if (PQG_LOG) pQgLog("pQgSelect-exists: |$sql|");	
				
				foreach($record as $field => $value) {
					$value = pQgStringQuotes($value, pQgGetType($field), 'PHP', 'GET');
					$sql = str_replace("$pQgAlias.$field", $value, $sql);
					$sql = str_replace("$pQgTable.$field", $value, $sql);	
				}
				
				$found = pQgGo('Recursion', $sql, 'Exists');
				
				if ( ( $exists['type'] == 'not exists' and  $found ) or
				     ( $exists['type'] == 'exists'     and !$found ) ) {

				    if (PQG_LOG) pQgLog("pQgSelect-delete: |$sql|");

				    unset ($result[$key]);
					
				}
				
			}

		}
	
	}

	$array = array ();
	$key = 0;
	foreach ( $result as $record ) {
		foreach($rFields as $field)
			$array [$key] [$field] = $record[$field];
		$key++;
	}
	return $array;

	print_r($array);
	
	$count = count($array);
	if (PQG_LOG) pQgLog("pQgSelect-end: $count");
	
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