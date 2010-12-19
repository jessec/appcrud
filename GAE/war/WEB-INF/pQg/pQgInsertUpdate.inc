<?php

function pQgSetFields($fields) {

	if (PQG_CHECK and ! trim($fields) )
		pQgError("Insert: No field=value list found");

	$parts = explode(',', pQgSaveFields($fields));

	foreach($parts as $key => $value )
		$parts[$key] = pQgSaveFieldsUndo($value);
		
	foreach($parts as $key => $value) {

		if (PQG_CHECK and ! strstr($value, '=') )
			pQgError("No '=' sign found in: $value");
		
		$field = str_replace('==', '=',  trim($value));
		$field = explode('=', $field);
			
		if (PQG_CHECK and count($field) > 2 )
			pQgError("To many '=' signs found in: $value");
		
		$combine [ trim( $field[0]) ] = $field[1];

	}
			
	return $combine;
	
}

function pQgUpdateObject($object, $fields, $option='') {

	if (PQG_LOG) pQgLog("pQgUpdateObject-start: |" . implode('|', $fields) . '|');
	
	foreach ($fields as $field => $value) {

		if (PQG_LOG) pQgLog("pQgUpdateObject-field: |$field|$value|");
		
		$set  = pQgGetSet($field);
		
		if (PQG_CHECK and !$set )
			pQgError("Field does not exists: (no set method found): $field");

		if ($option <> 'ASIS')
		   $value = pQgGetValue ($object, $field, $value, 'PHP');

		$object->$set($value) ;
				
		if (PQG_LOG) pQgLog("pQgUpdateObject-eval: |$set|$value|");		
		
	}
		
}

function pQgGetSet($field) {
	return 'set' . strtoupper(substr($field, 0, 1)) . substr($field, 1);
}

?>