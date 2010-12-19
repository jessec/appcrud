<?php

function pQgQuery($fields='') {

	global $pQgJDO, $pQgPMF, $pQgClass, $pQgTable, $pQgParts, $pQgExists;

	if (PQG_LOG) pQgLog("pQgQuery-start:");

	if ( isset ( $pQgParts['between'] ) )   pQgGo ('WhereBetween');
	if ( isset ( $pQgParts['in'] )      )   pQgGo ('WhereIn');

	$where = $group = $order = $range = '';

	$where = pQgOptions ( 'where'    );
	$order = pQgOptions ( 'order by' );
	$range = pQgOptions ( 'limit'    );

	if (PQG_LOG) pQgLog("pQgQuery-options: |$where|$order|$range|");

	$sql  = "SELECT $fields" ;
	$sql .= " FROM "     . $pQgClass;
	if ($where)  $sql .= " WHERE "    . $where;
	if ($order)  $sql .= " ORDER BY " . $order;

	$result = array();

	if (PQG_LOG) pQgLog("pQgQuery-basis: $sql");

	$queries   = array();
	$queries[] = $sql;

	if ($where) {
		if (strstr($where, ' || ')) $queries = pQgGo('WhereOR', $queries);
		if (strstr($where, ' != ')) $queries = pQgGo('WhereNE', $queries);
	}

	foreach ($queries as $sql) {

		$sql = pQgStringRedo($sql, 'JDO');
		 
		 
		if (PQG_LOG) $pQgJDO[] = $sql;

		if (PQG_LOG) pQgLog("pQgQuery-newQuery: $sql");

		if (PQG_JDO)
		try  { $query = $pQgPMF->newQuery($sql); }
		catch (Exception $e){ if (PQG_CHECK) pQgExceptionHandler($e); }

		if ($range)
		pQgRange($query, $range);

		if (PQG_JDO)
		try  { $objects = $query->execute();}
		catch  (Exception $e) { if (PQG_CHECK) pQgExceptionHandler($e); }

		if (!PQG_JDO) $objects = array();

		$count = count($objects);
		if (PQG_LOG) pQgLog("pQgQuery-loop: Found $count records");

		$result = array_merge($result, $objects);

	}

	if (!PQG_JDO) {
		$result[0][0] = 1234;
		$result[0][1] = '111';
		$result[0][2] = '222';
		$result[0][3] = '333';
		$result[0][4] = 1000;
		$result[1][0] = 4321;
		$result[1][1] = '444';
		$result[1][2] = '555';
		$result[1][3] = '666';
		$result[1][4] = 2000;
	}

	$count = count($result);

	if (PQG_LOG) pQgLog("pQgQuery-end: Found $count records");

	return $result;

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

	if ( PQG_JDO) $query->setRange($start, $count);

}

?>