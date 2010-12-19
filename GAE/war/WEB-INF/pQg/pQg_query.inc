<?php




function pQgQuery($fields='') {

	global $pQgPMF, $pQgClass;

	if (PQG_LOG) pQgLog("pQgQuery-start:");

	pQgBetween ();
	pQgIn ();

	$where = $group = $order = $range = '';

	$where = pQgOptions ( 'where'    );
	$order = pQgOptions ( 'order by' );
	$range = pQgOptions ( 'limit'    );


	if (PQG_LOG) pQgLog("pQgQuery-options: |$where|$order|$range|");

	$sql  = "SELECT $fields" ;
	$sql .= " FROM "     . $pQgClass;
	if ($where)  $sql .= " WHERE "    . $where;
	if ($order)  $sql .= " ORDER BY " . $order;

	$check = explode("'", $sql);
	$sql = '';
	foreach($check as $key => $value)
	if ($key % 2)  $sql .= pQgGetString($value, 'JDO');
	else           $sql .= $value;

	$result = array();

	if (PQG_LOG) pQgLog("pQgQuery-basis: $sql");

	$queries   = array();
	$queries[] = $sql;

	if ($where) {
		$queries   = pQgOR($queries);
		$queries   = pQgNotEqual($queries);
	}

	foreach ($queries as $sql) {

		if (PQG_LOG) pQgLog("pQgQuery-loop: $sql");

		$query = $pQgPMF->newQuery($sql);

		if ($range)
		pQgRange($query, $range);

		$objects = $query->execute();

		$count = count($objects);
		if (PQG_LOG) pQgLog("pQgQuery-loop: Found $count records");

		$result = array_merge($result, $objects);

	}

	$count = count($result);

	if (PQG_LOG) pQgLog("pQgQuery-end: Found $count records");

	return $result;

}

function pQgIn () {

	global  $pQgParse;

	$hit = FALSE;

	foreach( $pQgParse  as $key => $value ) {

		if ($value == 'in') {

			$hit = TRUE;
				
			$field = $pQgParse  [$key-1] ;
				
			$val = array();
			$len = count($pQgParse );
				
			for ($i=$key+2; $key<$len; $i++) {

				$pos = $pQgParse  [$i];

				if ( $pos <> ')' and $pos <> ',' )
				$val[] = $pos;

				unset ($pQgParse  [$i]) ;

				if ( $pos == ')' )

				break;
			}

			$in = "$field = " . implode ( " or $field == ", $val);

			if (PQG_LOG) pQgLog("pQgIn: $in");
				
			$pQgParse  [$key] = $in;
				
			unset ( $pQgParse  [$key-1] ) ;
			unset ( $pQgParse  [$key+1] ) ;
				
		}

	}

	if ($hit)
	pQgParse();

}


function pQgBetween () {

	global  $pQgParse;

	$hit = FALSE;

	foreach( $pQgParse  as $key => $value ) {

		if ($value == 'between') {

			$field = $pQgParse  [$key-1] ;
			$low   = $pQgParse  [$key+1] ;
			$and   = $pQgParse  [$key+2] ;
			$high  = $pQgParse  [$key+3] ;
				
			$new = "$field >= $low && $field <= $high";

			if (PQG_LOG) pQgLog("pQgBetween: $add");
				
			$pQgParse  [$key-1] = $new;
				
			unset ( $pQgParse  [$key]   ) ;
			unset ( $pQgParse  [$key+1] ) ;
			unset ( $pQgParse  [$key+2] ) ;
			unset ( $pQgParse  [$key+3] ) ;

			$hit = TRUE;
				
		}

	}

	if ($hit)
	pQgParse();

}

function pQgNotEqual($sql) {

	$queries = array();

	foreach($sql as $query) {

		if (PQG_LOG) pQgLog("pQgNotEqual-loop: $query");

		$pos = strpos($query, ' != ');
		while ($pos) {

			$before = substr ( $query, 0, $pos  );
			$after  = substr ( $query, $pos + 4 );

			$add1 = $before . ' < ' . $after;
			$add2 = $before . ' > ' . $after;
				
			if (PQG_LOG) pQgLog("pQgNotEqual-add-1: $add1");
			if (PQG_LOG) pQgLog("pQgNotEqual-add-2: $add2");
				
			$queries [] = $add1;
			$query      = $add2;
				
			$pos = strpos($query, ' != ');

		}

		$queries [] = $query;

	}

	return $queries;

}

function pQgOR($sql) {

	$queries = array();

	foreach($sql as $query) {

		if (PQG_LOG) pQgLog("pQgNotOR-loop: $query");

		$where = explode(' WHERE ', $query);
		if (PQG_CHECK and (! count($where) or count($where) > 2) )
		pQgError("pQgOR-Internal error: " . count($where));
			
		$or = explode(' || ', $where[1]);

		$always = '';
		foreach ($or as $key =>$or_part) {
			$and = explode(' && ', $or_part, 2);
			if ( count($and) > 1 ) {
				$or [$key] = str_replace(' && ' . $and[1], '', $or_part);
				$always .= ' && ' . $and[1] ;
			}
		}

		if ( count ($or) == 1)

		$queries [] = $query;

		else {
				
			foreach ($or as $or_part) {
				$add = $where [0] . " WHERE " . $or_part . $always;
				if (PQG_LOG) pQgLog("pQgNotOR-add: $add");
				$queries [] = $add;
			}
				
		}

	}

	return $queries;

}

function pQgRange($query, $range) {

	if (PQG_LOG) pQgLog("pQgRange-start: $range");

	if (PQG_CHECK and ! trim($range) )
	pQgError("No parameters found for LIMIT");

	if ( ! strstr($range, ',') )
	$range = "0 , $range";

	$parts = explode(',', $range);

	if (PQG_CHECK and count($parts) <> 2 )
	pQgError("Invalid LIMIT clause, more then 1 ',' chars found: $range");

	$start = trim($parts[0]);
	if (PQG_CHECK and ! is_int(intval($start)) )
	pQgError("Invalid LIMIT clause, start value is not an integer value: $start");

	$count = trim($parts[1]);
	if (PQG_CHECK and ! is_int(intval($count)) )
	pQgError("Invalid LIMIT clause, count value is not an integer value: $count");

	if (PQG_LOG) pQgLog("pQgRange: |$start|$count|");

	$query->setRange($start, $count);

}

?>